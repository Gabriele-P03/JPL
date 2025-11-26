/**
 * WAV file parser
 * 
 * Uncompressed high-quality audio
 */

#ifndef WAV_JPL
#define WAV_JPL

#include <fstream>
#include <jpl/exception/runtime/IllegalArgumentException.hpp>

namespace jpl{
    namespace _parser{
        namespace _wav{

            #pragma pack(push, wav, 1)

            struct RIFF{
                char chunkID[4];            //It is always 'RIFF'
                unsigned int chunkSize;     //It is the size of file without considering 8 bytes (chunkID + chunkSize)
                char format[4];             //IT is always 'WAVE'   
            };

            struct FMT{
                char subchunk1ID[4];        //It is always 'fmt ' (with space)
                unsigned int subchunk1Size; //16 for PCM otherwise it is the size of the rest of this subchunk
                unsigned short audioFormat; //PCM = 1 (i.e. Linear Quantization). != 1 => compression
                unsigned short numChannels; //Mono=1; Stereo=2 etc...
                unsigned int sampleRate;    //8000 Hz, 44100 Hz etc...
                unsigned int byteRate;      //sampleRate * numChannels * bitsPerSample/8
                unsigned short blockAlign;  //numChannels * bitsPerSample/8
                unsigned short bitsPerSample;
            };

            struct DATA{
                char subchunk2Id[4];        //It is always 'DATA'
                unsigned int subchunk2Size; //Bytes of data. NumSamples * NumChannels * BitsPerSample/8
                char* data;
            };

            struct WAV{
                RIFF riff;
                FMT fmt;
                DATA data;
            };

            #pragma pack(pop, wav)

            extern WAV* parse(std::fstream* file);

            extern void write(std::fstream* file, WAV* wav);
        }
    }
}

#endif