//-----------------------------------------------------------------------------
// Copyright (c) 2015 Andrew Mac
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <cstdint>

#include "Bitmap.h"

void writeBMP(const char* filename, uint8_t* data, int width, int height)
{
   std::ofstream bmpFile;
   bmpFile.open(filename, std::ios_base::binary);
   if ( !bmpFile.is_open() )
      return;

   // Determine endian-ness
   int num = 1;
   bool little_endian = *(char *)&num == 1;

   // Bytes per pixel
   uint32_t bytesPP = 3;

   // Row Padding
   uint32_t row_padding = 4 - ((width * bytesPP) % 4);
   row_padding = row_padding < 4 ? row_padding : 0;

   // Bitmap Info Header
   uint32_t biSize            = 40;
   int32_t  biWidth           = width;
   int32_t  biHeight          = height;
   uint16_t biPlanes          = 1;
   uint16_t biBitCount        = 24;
   uint32_t biCompression     = 0L;
   uint32_t biSizeImage       = (width * height * bytesPP) + (row_padding * height);
   int32_t  biXPelsPerMeter   = 0;
   int32_t  biYPelsPerMeter   = 0;
   uint32_t biClrUsed         = 0;
   uint32_t biClrImportant    = 0;

   // Bitmap File Header
   uint16_t bfType            = little_endian ? 19778 : 16973; // Type of file 'BM'
   uint32_t bfOffBits         = 40 + 14; // Size of Info + File Header
   uint32_t bfSize            = bfOffBits + biSizeImage;
   uint16_t bfReserved1       = 0;
   uint16_t bfReserved2       = 0;

   // Write File Header
   bmpFile.write(reinterpret_cast<const char *>(&bfType),          sizeof(bfType));
   bmpFile.write(reinterpret_cast<const char *>(&bfSize),          sizeof(bfSize));
   bmpFile.write(reinterpret_cast<const char *>(&bfReserved1),     sizeof(bfReserved1));
   bmpFile.write(reinterpret_cast<const char *>(&bfReserved2),     sizeof(bfReserved2));
   bmpFile.write(reinterpret_cast<const char *>(&bfOffBits),       sizeof(bfOffBits));

   // Write Info Header
   bmpFile.write(reinterpret_cast<const char *>(&biSize),          sizeof(biSize));
   bmpFile.write(reinterpret_cast<const char *>(&biWidth),         sizeof(biWidth));
   bmpFile.write(reinterpret_cast<const char *>(&biHeight),        sizeof(biHeight));
   bmpFile.write(reinterpret_cast<const char *>(&biPlanes),        sizeof(biPlanes));
   bmpFile.write(reinterpret_cast<const char *>(&biBitCount),      sizeof(biBitCount));
   bmpFile.write(reinterpret_cast<const char *>(&biCompression),   sizeof(biCompression));
   bmpFile.write(reinterpret_cast<const char *>(&biSizeImage),     sizeof(biSizeImage));
   bmpFile.write(reinterpret_cast<const char *>(&biXPelsPerMeter), sizeof(biXPelsPerMeter));
   bmpFile.write(reinterpret_cast<const char *>(&biYPelsPerMeter), sizeof(biYPelsPerMeter));
   bmpFile.write(reinterpret_cast<const char *>(&biClrUsed),       sizeof(biClrUsed));
   bmpFile.write(reinterpret_cast<const char *>(&biClrImportant),  sizeof(biClrImportant));

   // Write the bitmap bits, padded to multiples of 4 bytes.
   for (int32_t y = 0; y < height; y++)
   {
      // BMP expects bottom to top order on y axis.
      const uint8_t* pRow = data + (height - y - 1) * width * bytesPP;
  
      // Write each pixel. 
      for(int32_t x = 0; x < width; x++)
      {   
         if ( little_endian )
         {
            // Little endian writes in order: BGR.
            bmpFile.write(reinterpret_cast<const char *>(pRow + (x * bytesPP) + 2), 1);
            bmpFile.write(reinterpret_cast<const char *>(pRow + (x * bytesPP) + 1), 1);
            bmpFile.write(reinterpret_cast<const char *>(pRow + (x * bytesPP)), 1);
         } else {
            // Big endian writes in order: RGB.
            bmpFile.write(reinterpret_cast<const char *>(pRow + (x * bytesPP)), 1);
            bmpFile.write(reinterpret_cast<const char *>(pRow + (x * bytesPP) + 1), 1);
            bmpFile.write(reinterpret_cast<const char *>(pRow + (x * bytesPP) + 2), 1);
         }
      }

      // Write out empty bytes for row padding.
      for (uint32_t n = 0; n < row_padding; n++)
         bmpFile.write("", 1);
   }

   bmpFile.close();
}