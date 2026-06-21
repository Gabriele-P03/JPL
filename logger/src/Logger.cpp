#include "Logger.hpp"

jpl::_logger::Logger* jpl::_logger::Logger::INSTANCE = nullptr;

void jpl::_logger::Logger::initStaticLogger(){
    #ifdef __linux__
        jpl::_logger::Logger::INSTANCE = new jpl::_logger::Logger(
            jpl::_utils::_files::getLocalPath("logs/" + jpl::_logger::Logger::getFileNameOfInstance() + ".txt"), false, MAX_LOG_REQUEST_PER_FLUSH_JPL
        );
    #elif _WIN32
        jpl::_logger::Logger::INSTANCE = new jpl::_logger::Logger(
            jpl::_utils::_files::getLocalPath("logs\\" + jpl::_logger::Logger::getFileNameOfInstance() + ".txt"), false, MAX_LOG_REQUEST_PER_FLUSH_JPL
        );
    #endif
}

jpl::_logger::LOG_STATUS jpl::_logger::INFO_JPL = "INF";
jpl::_logger::LOG_STATUS jpl::_logger::WARNING_JPL = "WAR";
jpl::_logger::LOG_STATUS jpl::_logger::ERROR_JPL = "ERR";
jpl::_logger::LOG_STATUS jpl::_logger::DEBUG_JPL = "DBG";

void jpl::_logger::Logger::writeOnFile(jpl::_logger::Logger* const instance){
    std::unique_lock<std::mutex> lock(instance->logger_mutex);
    while(true){
        instance->cv.wait(lock, [&](){
            return instance->requestQueue.size() >= MAX_LOG_REQUEST_PER_FLUSH_JPL || instance->close;
        });
        std::queue<std::string>* q = &instance->requestQueue;
        while(!q->empty()){
            std::string msg = q->front();
            q->pop();
            instance->file->write(msg.c_str(), msg.size());
        }
        //No more logger request to satisfy
        instance->file->flush();
        if(instance->close)
            break;//writeOnFile called by destructor
    }
    instance->file->close();
    delete instance->file;
}

jpl::_logger::Logger::Logger(const std::string &pathToFile, bool quiet, size_t size)
    : size(size){
   this->flag = false;
   this->close = false;
   if(!quiet){
       this->file = new std::ofstream();
       this->file->open(pathToFile);
       if(file->fail()){
           std::cout<<"Could not create output file: "<<pathToFile<<std::endl;
       }else{
           this->flag = true;
           this->delegatedThread = std::thread(jpl::_logger::Logger::writeOnFile, this);
       }
   }
}


void jpl::_logger::Logger::print(const std::string& msg){
    this->print(msg, jpl::_logger::INFO_JPL);
}

void jpl::_logger::Logger::print(const std::string& msg, const jpl::_logger::LOG_STATUS status){
    if(status == jpl::_logger::DEBUG_JPL && !jpl::_utils::_debug::isDebugging())
        return;
    logger_mutex.lock();
    std::string finalMsg = "[" + this->getFileNameOfInstance() + " -> " + status + "]: " + msg + "\n";
    std::cout<<finalMsg;
    bool wakeUpFlag = false;
    if (this->flag){    //it has to store a new LoggerRequest
        this->requestQueue.push(finalMsg);
        if(this->requestQueue.size() >= this->size)
            wakeUpFlag = true;
    }
    logger_mutex.unlock();
    if (wakeUpFlag) //After unlock to prevent "Hurry Up and Wait"
        this->cv.notify_all();  //It's time to satisfy all request
}

void jpl::_logger::Logger::flush(){
    this->cv.notify_all();
}

void jpl::_logger::Logger::flushAndClose(){
    this->logger_mutex.lock();
    this->close = true;
    this->logger_mutex.unlock();
    this->cv.notify_all();
    this->delegatedThread.join();
}

jpl::_logger::Logger::~Logger(){
    this->flushAndClose();
}

std::string jpl::_logger::Logger::getFileNameOfInstance(){
    std::tm *ltm = jpl::_logger::Logger::getTM();
    return  std::to_string((*ltm).tm_mday) + "-" + 
            std::to_string((*ltm).tm_mon+1) + "-" + 
            std::to_string((*ltm).tm_year + 1900) + "_" +
            std::to_string((*ltm).tm_hour) + "-" +
            std::to_string((*ltm).tm_min) + "-" +
            std::to_string((*ltm).tm_sec);
}