#define AUTO_LOG_EXCEPTION_JPL
#define USE_STACKTRACE_W_EXCEPTION_JPL
#include <jpl/logger/LoggerWrapper.hpp>
#include <jpl/utils/FilesUtils.hpp>
#include "src/dtd/DTD.hpp"


using namespace jpl;
using namespace _parser;



int main(){

    std::ifstream* in = new std::ifstream("/home/gabriele/JPL/parser/out/linux/test.dtd");

    _dtd::parse(in);
}
