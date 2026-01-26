#include "src/wav/WAV.hpp"

using namespace jpl;
using namespace _parser;

int main(){

    std::fstream in;
    in.open("C:\\Users\\Utente\\JPL\\parser\\out\\windows\\wav_test.wav", std::ios_base::in|std::ios_base::binary);
    if(in.is_open()){
        printf("OK");
    }else{
        printf("Not opened");
    }
    _parser::_wav::WAV* wav = _parser::_wav::parse(&in);

    std::fstream* out = new std::fstream;
    out->open("C:\\Users\\Utente\\JPL\\parser\\out\\windows\\wav_outtest.wav", std::ios_base::out | std::ios_base::binary);
    jpl::_parser::_wav::write(out, wav);

}
