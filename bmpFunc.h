#ifndef HEADER_FILE
#define HEADER_FILE
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

#define DEBUG

/* BMP header information found at:
   https://web.archive.org/web/20080912171714/http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html
 */
#pragma pack(push, 1)
typedef struct {
    //______________________||_________________||_____||
    //Name                  ||stdvalue         ||bytes||
    //______________________||_________________||_____||
    uint16_t bfType;      //BM = 0x4d42  ||2
    uint32_t bfSize;      //?            ||4
    uint16_t bfReserved1; //0            ||2
    uint16_t bfReserved2; //0            ||2
    uint32_t bfOffbits;   //0x3E         ||4
} BMPFileHeader;

typedef struct {
    //______________________||______________||_____||
    //Name                  ||stdvalue      ||bytes||
    //______________________||______________||_____||
    uint32_t biSize;          //0x00  ||4
    uint32_t biWidth;         //0x00  ||4
    uint32_t biHeight;        //0x00  ||4
    uint16_t biPlanes;        //0x01  ||2
    uint16_t biBitCount;      //0x08  ||2
    uint32_t biCompression;   //0x00  ||4
    uint32_t biSizeImage;     //0x00  ||4
    uint32_t biXPelsPerMeter; //0x00  ||4
    uint32_t biYPelsPerMeter; //0x00  ||4
    uint32_t biClrUsed;       //0x00  ||4  uses bits per pixel define if you want other colors
    uint32_t biClrImportant;  //0x00  ||4  if all colors are important
} BMPInfoHeader;

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t pad;
} rgbQuad;
#pragma pack(pop)

void swap(rgbQuad a, rgbQuad b);
void verifyBMP(FILE* fPtr, BMPFileHeader* bmpFH, BMPInfoHeader* bmpIH);
rgbQuad* toCArr(FILE* fPtr, uint32_t pixels);
void invBMP(uint32_t pixels, rgbQuad* colors);
void tpArr(uint32_t* height, uint32_t* width, rgbQuad** CArr, rgbQuad** newCArr);
void reverseRow(uint32_t* height, uint32_t* width, rgbQuad** CArr);
rgbQuad** rotateBMP(uint32_t* height, uint32_t* width, rgbQuad** CArr);
void writeBMP(FILE* fPtr, BMPFileHeader* bmpFH, BMPInfoHeader* bmpIH, rgbQuad* CArr, uint32_t pixels);

#endif
