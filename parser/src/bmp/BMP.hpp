/**
 * 
 */

#ifndef BMP_JPL
#define BMP_JPL

#include <fstream>
#include <cmath>

namespace jpl{
    namespace _parser{
        namespace _bmp{

            #pragma pack(push, bmp, 1)

            struct Palette{
                unsigned char red;
                unsigned char green;
                unsigned char blue;
                unsigned char unused;
            };

            struct BMP{
                //Header
                unsigned char signature[2];  //'BM'
                unsigned int fileSize;    //File size in bytes
                unsigned char reserved[4];   //unused (=0)
                unsigned int dataOffset;  //Offset from beginning of file to the beginning of the bitmap data
                //InfoHeader
                int size;        //	Size of InfoHeader =40 
                unsigned int width;
                unsigned int height;
                unsigned short planes;      //number of planes (=1)
                /*
                    Bits per Pixel used to store palette entry information. This also identifies in an indirect way the number of 
                    possible colors. Possible values are:
                    1 = monochrome palette. NumColors = 1  
                    4 = 4bit palletized. NumColors = 16  
                    8 = 8bit palletized. NumColors = 256 
                    16 = 16bit RGB. NumColors = 65536
                    24 = 24bit RGB. NumColors = 16M
                */
                unsigned short bitsPerPixel; 
                /*
                	Type of Compression  
                    0 = BI_RGB   no compression  
                    1 = BI_RLE8 8bit RLE encoding  
                    2 = BI_RLE4 4bit RLE encoding
                    3 = RGB bitmap with mask
                */
                unsigned int compression;
                unsigned int imageSize;   //(compressed) size of image. It is valid to set this =0 if Compression = 0
                int xPixelPerM;  //horizontal resolution: Pixels/meter
                int yPixelPerM;  //vertical resolution: Pixels/meter
                unsigned int colorUsed; //Number of actually used colors. For a 8-bit / pixel bitmap this will be 100h or 256.
                unsigned int importantColors; //Number of important colors. 0 = all
                //Color Table
                Palette* palette;    //Repeated NumCOlors times
                char* data;             //Image data
            };
            #pragma pack(pop, bmp)

            BMP* read(std::fstream* stream);

            void write(std::fstream* s, BMP* bmp);

            char* toRender(BMP* bmp, size_t &size);
        }
    }
}

#endif