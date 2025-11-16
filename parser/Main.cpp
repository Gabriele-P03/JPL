#include "src/bmp/BMP.hpp"


using namespace jpl;
using namespace _parser;

int main(){

  std::fstream in;
    in.open("C:\\Users\\Utente\\JPL\\parser\\out\\windows\\ascii.bmp", std::ios_base::in|std::ios_base::binary);
    if(in.is_open()){
        printf("OK");
    }else{
        printf("Not opened");
    }
    _parser::_bmp::BMP* bmp = _parser::_bmp::read(&in);

    std::fstream* out = new std::fstream;
    out->open("C:\\Users\\Utente\\JPL\\parser\\out\\windows\\testout.bmp", std::ios_base::out | std::ios_base::binary);
    std::fstream* outReversed = new std::fstream;
    outReversed->open("C:\\Users\\Utente\\JPL\\parser\\out\\windows\\testoutrev.bmp", std::ios_base::out | std::ios_base::binary);
    
    jpl::_parser::_bmp::write(out, bmp);
    out->flush();
    out->close();

    size_t size;
    char* dataReversed = jpl::_parser::_bmp::toRender(bmp, size);
    outReversed->write((char*)bmp, 54);
    outReversed->write(dataReversed, size);
    outReversed->close();
}
