#include "Profiler.hpp"

unsigned long jpl::_utils::_profiler::Profiler::processors = 0;
unsigned long jpl::_utils::_profiler::Profiler::lastCPUTime = 0;
unsigned long jpl::_utils::_profiler::Profiler::lastKernelTime = 0;
unsigned long jpl::_utils::_profiler::Profiler::lastUserTime = 0;

#ifdef _WIN32
    PDH_HQUERY jpl::_utils::_profiler::Profiler::cpuQuery = nullptr;
    PDH_HCOUNTER jpl::_utils::_profiler::Profiler::cpuTotal = nullptr;

#elif __linux__
    FILE* jpl::_utils::_profiler::Profiler::procCpuinfo = nullptr;
    FILE* jpl::_utils::_profiler::Profiler::procLoadavg = nullptr;

    unsigned long jpl::_utils::_profiler::Profiler::parseKBLine(char *line) const{
        int sizeLine = strlen(line);
        const char *p = line;
        while (*p < '0' || *p > '9') // Slide pointer 'till a digit is found
            p++;
        line[sizeLine - 3] = '\0'; //-3 since the line will over with " Kb"
        return std::stoi(p);
    }
#endif

jpl::_utils::_profiler::SystemInfo::~SystemInfo(){delete[] this->procsCPUMhz;}

jpl::_utils::_profiler::Profiler::Profiler(){
    this->init();
    this->started = false;
    this->sleepMS = 0;
}

void jpl::_utils::_profiler::Profiler::init(){
    #ifdef _WIN32
        if(jpl::_utils::_profiler::Profiler::cpuQuery == nullptr){
            PdhOpenQuery(NULL, NULL, &jpl::_utils::_profiler::Profiler::cpuQuery);
            PdhAddEnglishCounter(jpl::_utils::_profiler::Profiler::cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &jpl::_utils::_profiler::Profiler::cpuTotal);
            PdhCollectQueryData(jpl::_utils::_profiler::Profiler::cpuQuery);
        }
        if(jpl::_utils::_profiler::Profiler::processors == 0){
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            jpl::_utils::_profiler::Profiler::processors = sysInfo.dwNumberOfProcessors;
        }
        this->self = GetCurrentProcess();
        FILETIME ftime, fkernel, fuser;
        GetSystemTimeAsFileTime(&ftime);
        memcpy(&this->lastPCPU, &ftime, sizeof(FILETIME));
        GetProcessTimes(this->self, &ftime, &ftime, &fkernel, &fuser);
        memcpy(&this->lastPKernelTime, &fkernel, sizeof(FILETIME));
        memcpy(&this->lastPUserTime, &fuser, sizeof(FILETIME));

    #elif __linux__
        this->procSelfStatus = fopen("/proc/self/status", "r");

        if(jpl::_utils::_profiler::Profiler::procLoadavg == nullptr)
            jpl::_utils::_profiler::Profiler::procLoadavg = fopen("/proc/loadavg", "r");
        if(jpl::_utils::_profiler::Profiler::procCpuinfo == nullptr)
            jpl::_utils::_profiler::Profiler::procCpuinfo = fopen("/proc/cpuinfo", "r");

        if(jpl::_utils::_profiler::Profiler::processors == 0){
            //Let's count how many processors units are available
            char buffer[256];
            while( fgets(buffer, 256, jpl::_utils::_profiler::Profiler::procCpuinfo) != NULL ){
                if( strncmp(buffer, "processor", 9) == 0){
                    jpl::_utils::_profiler::Profiler::processors++;
                }
            }
            rewind(jpl::_utils::_profiler::Profiler::procCpuinfo);  //Rewinding /proc/cpuinfo
        }

        timeSample tmSample;
        jpl::_utils::_profiler::Profiler::lastCPUTime = times(&tmSample);
        jpl::_utils::_profiler::Profiler::lastKernelTime = tmSample.tms_stime;
        jpl::_utils::_profiler::Profiler::lastUserTime = tmSample.tms_utime;
    #endif
}

void jpl::_utils::_profiler::Profiler::measureMemory(jpl::_utils::_profiler::SystemInfo *&systemInfo) const{
    #ifdef _WIN32
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;

        systemInfo->totalMemory = memInfo.ullTotalPhys; //Total Physical Memory
        systemInfo->freeMemory = memInfo.ullAvailPhys;
        systemInfo->usedMemory = systemInfo->totalMemory - systemInfo->freeMemory;

        systemInfo->virtualTotalMemory = memInfo.ullTotalVirtual;
        systemInfo->virtualFreeMemory = memInfo.ullAvailVirtual;
        systemInfo->virtualUsedMemory = memInfo.ullTotalVirtual - memInfo.ullAvailVirtual;

        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        systemInfo->virtualProcMemory = pmc.PrivateUsage;
        systemInfo->procMemory = pmc.WorkingSetSize;

    #elif __linux__
        SysInfo memInfo;
        sysinfo(&memInfo);
        systemInfo->upTime = memInfo.uptime;
        systemInfo->totalMemory = memInfo.totalram;
        systemInfo->freeMemory = memInfo.freeram;
        systemInfo->usedMemory = systemInfo->totalMemory - systemInfo->freeMemory;

        systemInfo->virtualTotalMemory = memInfo.totalswap * memInfo.mem_unit;
        systemInfo->virtualFreeMemory = memInfo.freeswap * memInfo.mem_unit;
        systemInfo->virtualUsedMemory = (memInfo.totalswap - memInfo.freeswap) * memInfo.mem_unit;

        char buffer[128];
        bool flagVirtual = false, flagPhisically = false;
        while (fgets(buffer, 128, this->procSelfStatus) != NULL)
        {
            if (strncmp(buffer, "VmSize:", 7) == 0)
            {
                systemInfo->virtualProcMemory = this->parseKBLine(&buffer[0]);
                flagVirtual = true;
            }
            else if (strncmp(buffer, "VmRSS:", 6) == 0)
            {
                systemInfo->procMemory = this->parseKBLine(&buffer[0]);
                flagPhisically = true;
            }
            if (flagVirtual && flagPhisically)
                break;
        }
        rewind(this->procSelfStatus);
    #endif
}

void jpl::_utils::_profiler::Profiler::measureCpu(jpl::_utils::_profiler::SystemInfo* &systemInfo){
    #ifdef _WIN32
        //Total CPU
        PDH_FMT_COUNTERVALUE fmt;
        PdhCollectQueryData(jpl::_utils::_profiler::Profiler::cpuQuery);
        PdhGetFormattedCounterValue(jpl::_utils::_profiler::Profiler::cpuTotal, PDH_FMT_DOUBLE, NULL, &fmt);
        systemInfo->totalCpu = fmt.doubleValue;
        //CPU used by this processor
        FILETIME ftime, fkernel, fuser;
        ULARGE_INTEGER now, kernel, user;
        GetSystemTimeAsFileTime(&ftime);
        memcpy(&now, &ftime, sizeof(FILETIME));
        GetProcessTimes(this->self, &ftime, &ftime, &fkernel, &fuser);
        memcpy(&kernel, &fkernel, sizeof(FILETIME));
        memcpy(&user, &fuser, sizeof(FILETIME));
        systemInfo->procCpu = (100.0f/jpl::_utils::_profiler::Profiler::processors)*(user.QuadPart - this->lastPUserTime.QuadPart + kernel.QuadPart - this->lastPKernelTime.QuadPart)/(now.QuadPart - this->lastPCPU.QuadPart);
        this->lastPCPU = now;
        this->lastPKernelTime = kernel;
        this->lastPUserTime = user;
        //Cores freq
        PROCESSOR_POWER_INFORMATION* coresInfo = new PROCESSOR_POWER_INFORMATION[jpl::_utils::_profiler::Profiler::processors];
        CallNtPowerInformation(ProcessorInformation, NULL, 0, coresInfo, sizeof(PROCESSOR_POWER_INFORMATION)*jpl::_utils::_profiler::Profiler::processors);
        for(unsigned long i = 0; i < jpl::_utils::_profiler::Profiler::processors; i++){
            systemInfo->procsCPUMhz[i] = coresInfo[i].CurrentMhz;
        }
        delete[] coresInfo;
    #elif __linux__

        /*
            READING MHZ OF EACH CPU UNIT
        */
        unsigned long sizeStr = 256, i = 0, c = 0;
        while(true){
            char* buffer = new char[sizeStr];
            c = getline(&buffer, &sizeStr, jpl::_utils::_profiler::Profiler::procCpuinfo);
            if(c == 0 || c > sizeStr){
                delete[] buffer;
                break;
            }
            if( strncmp(buffer, "cpu MHz", 7) == 0){
                char* p = &buffer[c-2];
                while( *p == '.' || (*p >= '0' && *p <= '9'))
                    p--;
                systemInfo->procsCPUMhz[i++] = atoi(p);
            }
            delete[] buffer;
        }
        rewind(jpl::_utils::_profiler::Profiler::procCpuinfo);

        /*
            READING TOTAL PERCENTAGE OF USED CPU 
        */
        i = 0;
        sizeStr = 256;
        char* buffer = new char[sizeStr];
        c = getline(&buffer, &sizeStr, jpl::_utils::_profiler::Profiler::procLoadavg);
        if(c == 0 || c > sizeStr)
            systemInfo->totalCpu = 0;
        else{
            std::string str(buffer);
            jpl::_utils::_collections::_list::LinkedList<std::string> *buffer = jpl::_utils::_string::split(str, std::regex(" "));
            systemInfo->totalCpu = atof(buffer->get(0).c_str())*100.0/jpl::_utils::_profiler::Profiler::processors;
            delete buffer;
        }
        delete[] buffer;
        rewind(jpl::_utils::_profiler::Profiler::procLoadavg);

        /*
            READING PERCENTAGE OF USED CPU BY CURRENT APPLICATION
        */
        timeSample tmSample;
        long now = times(&tmSample);
        double percent;
        if(now == -1)
            throw jpl::_exception::RuntimeException("Could not read CPU usage of current process: " + jpl::_utils::_error::_GetLastErrorAsString());
        if(now <= jpl::_utils::_profiler::Profiler::lastCPUTime || tmSample.tms_stime < jpl::_utils::_profiler::Profiler::lastKernelTime || tmSample.tms_utime < jpl::_utils::_profiler::Profiler::lastUserTime){
            percent = (jpl::_utils::_profiler::Profiler::lastKernelTime+jpl::_utils::_profiler::Profiler::lastUserTime)/jpl::_utils::_profiler::Profiler::lastCPUTime;
        }else{
            percent = tmSample.tms_stime-jpl::_utils::_profiler::Profiler::lastKernelTime;
            percent+= tmSample.tms_utime-jpl::_utils::_profiler::Profiler::lastUserTime;
            percent/= (now - jpl::_utils::_profiler::Profiler::lastCPUTime);
            jpl::_utils::_profiler::Profiler::lastCPUTime = now;
            jpl::_utils::_profiler::Profiler::lastKernelTime = tmSample.tms_stime;
            jpl::_utils::_profiler::Profiler::lastUserTime = tmSample.tms_utime;
        }
        percent /= jpl::_utils::_profiler::Profiler::processors;
        percent *= 100.0f;
        systemInfo->procCpu = percent;
    #endif
}

const jpl::_utils::_profiler::SystemInfo *const jpl::_utils::_profiler::Profiler::measure(){
    SystemInfo *memInfo = new SystemInfo;
    memInfo->procsCPUMhz = new unsigned long[jpl::_utils::_profiler::Profiler::processors];
    memset(memInfo->procsCPUMhz, 0, sizeof(unsigned long)*jpl::_utils::_profiler::Profiler::processors);
    memInfo->time = time(NULL);
    #ifdef _WIN32
        memInfo->upTime = GetTickCount64();
    #endif
    this->measureMemory(memInfo);
    this->measureCpu(memInfo);
    return memInfo;
}

void* jpl::_utils::_profiler::Profiler::measures(void* instanceProfiler){
    jpl::_utils::_profiler::Profiler* profiler = static_cast<jpl::_utils::_profiler::Profiler*>(instanceProfiler);
    if( profiler == nullptr)
        throw jpl::_exception::IllegalArgumentException("Not a instance of Profiler class has been passed");
    #ifdef _WIN32
        unsigned long ms = profiler->sleepMS;
    #elif __linux__
        unsigned long ms = profiler->sleepMS*1000;
    #endif
    std::vector<const jpl::_utils::_profiler::SystemInfo*>* &list = profiler->systemInfoList;
    while (profiler->started){
        const jpl::_utils::_profiler::SystemInfo* const measureInfo = profiler->measure();
        list->push_back(measureInfo);
        
        #ifdef _WIN32
            Sleep(profiler->sleepMS);
        #elif __linux__
            usleep(ms);
        #endif
    }
    return nullptr;
}


void jpl::_utils::_profiler::Profiler::start(unsigned long sleepMS){
    if(this->started)
        throw jpl::_exception::IllegalStateException("This profiler has been already started");
    this->systemInfoList = new std::vector<const jpl::_utils::_profiler::SystemInfo*>();
    this->sleepMS = sleepMS;
    this->threadProfiler = new std::thread(&jpl::_utils::_profiler::Profiler::measures, this);
    this->started = true;
    try{
        this->threadProfiler->detach();
    }catch(const std::system_error* ex){
        this->started = false;
        throw jpl::_exception::RuntimeException("This profiler could not be started: " + jpl::_utils::_error::_GetLastErrorAsString(ex->code().value()));
    }

}

void jpl::_utils::_profiler::Profiler::end(){
    if(!this->started)
        throw jpl::_exception::IllegalStateException("This profiler has been already over");
    this->started = false;
    free(this->threadProfiler);
}

unsigned long jpl::_utils::_profiler::Profiler::getCoresAmount() {return jpl::_utils::_profiler::Profiler::processors;}
const std::vector<const jpl::_utils::_profiler::SystemInfo*>* jpl::_utils::_profiler::Profiler::getSystemInfoList() const{ return this->systemInfoList;}

jpl::_utils::_profiler::Profiler::~Profiler(){
    #ifdef _WIN32
        CloseHandle(this->self);
    #elif __linux__
        fclose(this->procSelfStatus);
    #endif
    delete this->systemInfoList;
}

bool jpl::_utils::_profiler::Profiler::isStarted() const noexcept{ return this->started;}
unsigned long jpl::_utils::_profiler::Profiler::getSleepMS() const noexcept{ return this->sleepMS;}