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
   unsigned int bytesToCopy = bmp->bitsPerPixel/8;
   char* buffer = new char[bmp->width*bmp->height*bytesToCopy];
   for(long h = bmp->height-1; h >= 0; h--){
      unsigned long pos = (bmp->height-h-1)*bmp->width*bytesToCopy;
      unsigned long pos1 = h*bmp->width*bytesToCopy;
      memcpy(&buffer[pos], &bmp->data[pos1], bmp->width*bytesToCopy);
   }
   size = bmp->height*bmp->width*bytesToCopy;
   return buffer;
   /*unsigned short bytesToCopy = bmp->bitsPerPixel/8;
   unsigned short diff = bmp->imageSize/(bmp->width*bmp->height);
   char* buffer = new char[bmp->height*bmp->width*bytesToCopy];   //Allocate buffer h*w*bytesToCopy
   size_t count = 0;
   for(long h = bmp->height-1; h >= 0; h--){
      for(long w = 0; w < bmp->width*diff; w += diff){
         size_t pos = h*bmp->width+w;  //Pos of the bytesToCopy set
         char* src = &bmp->data[pos];
         char* dst = &buffer[count];
         memcpy(dst, src, bytesToCopy);
         count += bytesToCopy;
      }
   }
   size = count;
   return buffer;
   */
}