/**
 * @file
 * 
 * A timer is a tool which, based on start-timestamp caught via start() method and the one via end(),
 * describes the time-line of a function or a set of calls
 * 
 *
 * @date 2024-01-14
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */
#ifndef TIMER_JPL
#define TIMER_JPL

#include <ctime>
#include <jpl/logger/Logger.hpp>
#include <jpl/exception/runtime/IllegalStateException.hpp>

namespace jpl{
    namespace _utils{
        namespace _timer{

            class Timer{

                protected:

                    bool terminated;
                    bool started;
                    std::string timerName;
                    time_t startTime, endTime;

                    
                    virtual std::string timeToString(const time_t *time) const{
                        #ifdef NUSE_ASCTIME_TIMER_JPL
                            return std::to_string(*time);
                        #else
                            return std::asctime(std::localtime(time));
                        #endif
                    }

                public:
                    Timer(std::string timerName){
                        this->timerName = timerName;
                        this->started = false;
                        this->terminated = false;
                        this->startTime = 0;
                        this->endTime = 0;
                    }
                    Timer() : Timer("unnamed"){
                    }

                    virtual void start() {
                        if(this->started)
                            throw new _exception::IllegalStateException("The timer " + this->timerName + " has been already started at " + this->timeToString(&this->startTime));
                        if(this->terminated)
                            throw new _exception::IllegalStateException("The timer " + this->timerName + " has been already terminated at " + this->timeToString(&this->endTime));
            
                        this->startTime = time(nullptr);
                        this->started = true;
                        _logger::print("Timer " + this->timerName + " started at " + this->timeToString(&this->startTime));
                    }

                    virtual void end(){
                        if(!this->started)
                            throw new _exception::IllegalStateException("The timer " + this->timerName + " has not been started yet");
                        if(this->terminated)
                            throw new _exception::IllegalStateException("The timer " + this->timerName + " has been already terminated at " + this->timeToString(&this->endTime));
            
                        this->endTime = time(nullptr);
                        this->started = false;
                        this->terminated = true;
                        _logger::print("Timer " + this->timerName + " finished at " + this->timeToString(&this->endTime));
                    }

                    virtual bool isStarted() const{return this->started;}
                    virtual bool isTerminated() const{return this->terminated;}
                    virtual const std::string* const getName() const {return &this->timerName;} 
                    virtual const time_t* const getStartTime() const {return &this->startTime;}
                    virtual const time_t* const getEndTime() const {return &this->endTime;}

            };
        }
    }
}



#endif