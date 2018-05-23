#ifndef HEADER_FILE
#define HEADER_FILE
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

// found at https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/Typeof.html#Typeof
#define max(a,b)                                \
  ({ typeof(a) _a = (a);                        \
    typeof(b) _b = (b);                         \
    _a > _b ? _a : _b; })

#define min(a,b)                                \
  ({ typeof(a) _a = (a);                        \
    typeof(b) _b = (b);                         \
    _a < _b ? _a : _b; })

#define swap(x,y) do{ x=x^y; y=x^y; x=y^x;} while(0);

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
#pragma pack(pop)

void verifyBMP(FILE* fPtr, BMPFileHeader* bmpFH, BMPInfoHeader* bmpIH);
void* toCArr(FILE* fPtr, BMPInfoHeader bmpIH, uint8_t* cArr);
void invBMP(BMPInfoHeader bmpIH, uint8_t* colors);
uint8_t* vFlip(uint32_t H, uint32_t rowSize, uint8_t* cArr);
void flipRow(uint8_t* row, uint32_t W);
uint8_t* hFlip(uint32_t H, uint32_t rowSize, uint32_t W, uint8_t* cArr);
uint8_t* rotateBMP90(BMPInfoHeader* bmpIH, BMPFileHeader* bmpFH, uint32_t* rSize, uint8_t* cArr, char* orientation);
uint8_t* rotateBMP180(BMPInfoHeader bmpIH, uint8_t* cArr, uint32_t rowSize);
void writeBMP(FILE* fPtr, BMPFileHeader* bmpFH, BMPInfoHeader* bmpIH, uint8_t* CArr);
void sepia(uint8_t* cArr, BMPInfoHeader bmpInfo, uint32_t rSize);

#endif
