#include "WAV.hpp"

jpl::_parser::_wav::WAV* jpl::_parser::_wav::parse(std::fstream* file){
    if(file == nullptr){
        throw jpl::_exception::IllegalArgumentException("WAV file is nullptr");
    }
    if(!file){
        throw jpl::_exception::IllegalArgumentException("WAV file is invalid");
    }
    jpl::_parser::_wav::WAV* wav = new jpl::_parser::_wav::WAV();
    size_t sizeReadyToRead = sizeof(jpl::_parser::_wav::WAV)-sizeof(jpl::_parser::_wav::DATA::data);
    char* startPoint = (char*)wav;
    file->read(startPoint, sizeReadyToRead);
    wav->data.data = new char[wav->data.subchunk2Size];
    char* data = wav->data.data;
    file->read(data, wav->data.subchunk2Size);
    return wav;
}

void jpl::_parser::_wav::write(std::fstream* file, jpl::_parser::_wav::WAV* wav){
    if(file == nullptr){
        throw jpl::_exception::IllegalArgumentException("WAV file is nullptr");
    }
    if(!file){
        throw jpl::_exception::IllegalArgumentException("WAV file is invalid");
    }
    size_t sizeReadyToBeWritten = sizeof(jpl::_parser::_wav::WAV)-sizeof(jpl::_parser::_wav::DATA::data);
    char* startPoint = (char*)wav;
    file->write(startPoint, sizeReadyToBeWritten);
    file->write(wav->data.data, wav->data.subchunk2Size);
    file->flush();
}