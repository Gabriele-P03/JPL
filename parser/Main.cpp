#define AUTO_LOG_EXCEPTION_JPL
#define USE_STACKTRACE_W_EXCEPTION_JPL
#include <jpl/logger/LoggerWrapper.hpp>
#include <jpl/utils/FilesUtils.hpp>
#include "src/dtd/DTD.hpp"


using namespace jpl;
using namespace _parser;



int main(){

    std::ifstream in;
    in.open("C:\\Users\\Utente\\JPL\\parser\\out\\linux\\test.dtd", std::ios_base::in);
    if(in.is_open()){
        printf("OK");
    }
    _dtd::parse(in);
}
