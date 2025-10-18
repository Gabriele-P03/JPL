#include "BMP.hpp"

#include <iostream>

jpl::_parser::_bmp::BMP* jpl::_parser::_bmp::read(std::fstream* bmpstream){

   jpl::_parser::_bmp::BMP* bmp = new jpl::_parser::_bmp::BMP;
   bmpstream->read( (char*)bmp, 54);
   size_t imageSize = bmp->imageSize;
   if(bmp->bitsPerPixel != 24){  //BMP with 24bit does not have any palette
      unsigned int numColors = pow(2, bmp->bitsPerPixel);
      bmp->palette = new jpl::_parser::_bmp::Palette[numColors];
      bmpstream->read((char*)bmp->palette, sizeof(jpl::_parser::_bmp::Palette)*numColors);
      imageSize -= numColors;
   }

   bmp->data = new char[imageSize];
   bmpstream->read(bmp->data, imageSize);

   unsigned int state = bmpstream->rdstate();
   return bmp;
}

void jpl::_parser::_bmp::write(std::fstream* s, jpl::_parser::_bmp::BMP* bmp){
   s->write((char*)bmp, 54);
   if(bmp->bitsPerPixel != 24){
      unsigned int numColors = pow(2, bmp->bitsPerPixel);
      s->write((char*)bmp->palette, sizeof(jpl::_parser::_bmp::Palette)*numColors);
   }
   s->write(bmp->data, bmp->imageSize);
}

char* jpl::_parser::_bmp::toRender(jpl::_parser::_bmp::BMP* bmp, size_t &size){
   unsigned short bytesToCopy = bmp->bitsPerPixel/8;
   unsigned short diff = bmp->imageSize/(bmp->width*bmp->height);
   char* buffer = new char[bmp->height*bmp->width*bytesToCopy];
   size_t count = 0;
   for(long i = bmp->imageSize-diff; i >= 0; i -= diff){
      char* dst = &buffer[count++*bytesToCopy];
      char* src = &bmp->data[i];
      memcpy(dst, src, bytesToCopy);
   }
   size = count*bytesToCopy;
   return buffer;
}