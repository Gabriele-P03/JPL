/**
 * A profiler can be used to catch global,current program and current thread memory consumption;
 * it can run either single-time or as a scheduled thread 
 * 
 * Usually we represents the CPU usage as a percentage of elapsed time since process started.
 * So the total CPU usage of an application is the sum of utime and stime, divided by elapsed time 
 * 
 * @date 2024-02-21
 * 
 * @author Gabriele-P03
*/

#ifndef PROFILER_JPL
#define PROFILER_JPL

    #include <ctime>
    #include "../StringUtils.hpp"
    #include <stdio.h>
    #include <string.h>
    #include <thread>
    #include <iostream>
    #include <jpl/exception/runtime/IllegalStateException.hpp>
    #include <jpl/exception/runtime/IllegalArgumentException.hpp>
    #include <jpl/utils/debug/ErrorUtils.hpp>
    #include <jpl/utils/ConversionUtils.hpp>

    #ifdef _WIN32
        #include <windows.h>
        #include <psapi.h>
    #elif __linux__
        #include <sys/sysinfo.h>
        #include <unistd.h>
    #endif

    namespace jpl{
        namespace _utils{
            namespace _profiler{

                struct SystemInfo;

                typedef struct sysinfo SysInfo;

                class Profiler{

                    protected:

                        static unsigned long processors;    //Contains the amount of CPU unit available

                        #ifdef __linux__
                            FILE* procSelfStatus;
                            FILE* procLoadavg;
                            FILE* procCpuinfo;
                            FILE* procSelfStat;
                        #elif _WIN32
                            static unsigned long lastIdleTicks, lastTotalTicks; 
                        #endif 

                        /**
                         * Parse the single line in order to retrieve what is before " Kb"  
                        */
                        virtual unsigned long parseKBLine(char* line) const;

                        /**
                         * It probes global and local (this proc) memory consumption such as
                         * total memory, total used memory, free one and used one by this proc
                        */
                        virtual void measureMemory(SystemInfo* &systemInfo) const;

                        /**
                         * It probes global and local (this proc) cpu consumption such as
                         * total cpu percentage, Mhz of each unit and percentage of this proc
                        */
                        virtual void measureCpu(SystemInfo* &systemInfo) const;

                        virtual void init();

                        std::vector<const SystemInfo*>* systemInfoList;
                        static void* measures(void* instanceProfiler);
                        unsigned long sleepMS;
                        bool started;
                        std::thread* threadProfiler;

                    public:

                        Profiler();
                        ~Profiler();

                        virtual void start(unsigned long sleepMS);
                        virtual void end();

                        virtual const SystemInfo* const measure() const;
                        const std::vector<const SystemInfo*>* getSystemInfoList() const;

                        static unsigned long getCoresAmount();
                };

                struct SystemInfo{

                    unsigned long time;         //Time as the SystemInfo stuct has been created at
                    unsigned long upTime;       //Seconds since boot

                    unsigned long totalMemory;  //Total RAM
                    unsigned long freeMemory;   //Free RAM at time
                    unsigned long usedMemory;   //RAM used globally
                    unsigned long procMemory;   //RAM used by the current process
                    unsigned long virtualTotalMemory;
                    unsigned long virtualFreeMemory;
                    unsigned long virtualUsedMemory;
                    unsigned long virtualProcMemory;

                    float totalCpu;     //Total Used CPU percentage 
                    unsigned long *procsCPUMhz; // Mhz of each CPU unit
                    float procCpu;      //Used CPU percentage by this process 

                    ~SystemInfo();

                    inline friend std::ostream& operator<<(std::ostream& os, const SystemInfo &systemInfo){
                        long tm_long;
                        long* tm_plong = &tm_long;
                        _conversions::ulong_long(systemInfo.time, tm_plong);
                        const time_t ptime_t = tm_long;
                        std::string tmp = std::asctime(std::localtime(&ptime_t));
                        tmp.pop_back();
                        os<<std::endl<<"["<<tmp<<"] - Uptime: "<<systemInfo.upTime<<")"<<std::endl;
                        os<<"| Global Physical Memory( Total: "<<systemInfo.totalMemory<<", Free: "<<systemInfo.freeMemory<<", Used: "<<systemInfo.usedMemory<<")"<<std::endl;
                        os<<"| Global Virtual Memory( Total: "<<systemInfo.virtualTotalMemory<<", Free: "<<systemInfo.virtualFreeMemory<<", Used: "<<systemInfo.virtualUsedMemory<<")"<<std::endl;
                        os<<"| Process' Used Memory( Physic: "<<systemInfo.procMemory<<", Virtual: "<<systemInfo.virtualProcMemory<<")"<<std::endl;
                        os<<"| CPU Percentage( Total: "<<systemInfo.totalCpu<<", Process': "<<systemInfo.procCpu<<")"<<std::endl;
                        os<<"| CPU Core MHz( ";
                        unsigned long size = jpl::_utils::_profiler::Profiler::getCoresAmount();
                        for(unsigned long i = 0; i < size; i++){
                            os<<std::to_string(i+1)<<"°: "<<systemInfo.procsCPUMhz[i];
                            if(i < size-1)
                                std::cout<<", ";
                        }
                        os<<")"<<std::endl;
                        return os;
                    }
                };

            }
        }
    }

#endif