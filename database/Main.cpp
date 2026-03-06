#define AUTO_LOG_EXCEPTION_JPL
#include "src/DBConnection.hpp"

#include "src/POJOTest.hpp"

int main(){
    jpl::_logger::_exceptionhook::LoggerExceptionHook();
    jpl::_db::DBConnection("localhost", "3306", "test", "root", "admin", SA_MySQL_Client);

    jpl::_db::POJOTest();
}