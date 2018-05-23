#include "bmpFunc.h"

// Verify and print the bmp headers
void verifyBMP(FILE* fPtr, BMPFileHeader* bmpFH, BMPInfoHeader* bmpIH) {
     fread(bmpFH, sizeof(*bmpFH), 1, fPtr);
     fread(bmpIH, sizeof(*bmpIH), 1, fPtr);
     // read filetype
     printf("Filetype is %"PRIx16"\n", bmpFH->bfType);
     if(bmpFH->bfType != 0x4d42) {
          printf("Not a bitmap\n");
          exit(-1);
     }
     // filesize
     printf("Filesize is %"PRIu32" bytes\n", bmpFH->bfSize);
     // offset
     printf("Offset is %"PRIu32" bytes\n", bmpFH->bfOffbits);
     // infoheader
     printf("Info header size is %"PRIu32" bytes\n", bmpIH->biSize);
     // fileheader has to be 40 bytes in size
     if(bmpIH->biSize != 40) {
          printf("Not a bitmap\n");
          exit(-1);
     }
     // width and height
     printf("Width is %"PRIu32" and height is %"PRIu32"\n", bmpIH->biWidth,
            bmpIH->biHeight);
     // planes
     printf("Amount of color planes is %"PRIu16"\n", bmpIH->biPlanes);
     // bits per pixel
     printf("Bit per pixel: %"PRIu16"\n", bmpIH->biBitCount);
     // compression
     printf("Compression type is %"PRIu32"\n", bmpIH->biCompression);
     // bitmap size
     printf("Bitmap size is %"PRIu32"\n", bmpIH->biSizeImage);
     // resolution
     printf("Resolution is %"PRIu32" by %"PRIu32"\n", bmpIH->biXPelsPerMeter,
            bmpIH->biYPelsPerMeter);
     // used colors
     printf("Number of used colors are %"PRIu32"\n", bmpIH->biClrUsed);
     // important colors
     printf("Numer of important colors are %"PRIu32"\n", bmpIH-> biClrImportant);
}

// Convert the image data to a color array
void* toCArr(FILE* fPtr, BMPInfoHeader bmpIH, uint8_t* cArr) {
     fread(cArr, bmpIH.biSizeImage, 1, fPtr);
     return cArr;
}

// Invert the image
void invBMP(BMPInfoHeader bmpIH, uint8_t* colors) {
     for(uint32_t i=0; i<bmpIH.biSizeImage; i++) {
          colors[i] = ~(colors[i]);
     }
}

// Function to flip an array vertically
uint8_t* vFlip(uint32_t H, uint32_t rowSize, uint8_t* inArr) {
     uint8_t* tempBuf;
     if((tempBuf = malloc(rowSize)) == NULL) {
          printf("Invalid address\n");
          exit(-1);
     }
     for(uint32_t i = 0; i < H/2; i++) {
          memcpy(tempBuf, inArr+(i*rowSize), rowSize);
          memcpy(inArr+(i*rowSize), inArr+((H-i-1)*rowSize), rowSize);
          memcpy(inArr+((H-i-1)*rowSize), tempBuf, rowSize);
     }
     free(tempBuf);
     return inArr;
}

void flipRow(uint8_t* row, uint32_t W) {
     for(uint32_t i = 0; i < W/2; i+=3) {
          swap(*(row+i+0),*(row+W-1-i-2));
          swap(*(row+i+1),*(row+W-1-i-1));
          swap(*(row+i+2),*(row+W-1-i-0));
     }
}

// Function to flip an array horizontally
uint8_t* hFlip(uint32_t H, uint32_t rowSize, uint32_t W, uint8_t* inArr) {
     for(uint32_t i = 0; i < H; i++) {
          flipRow(inArr+i*rowSize, W*3);
     }
     return inArr;
}


// Function to rotate an image 90 degrees cw or ccw
// There is still an error when trying to rotate multiple times
uint8_t* rotateBMP90(BMPInfoHeader* bmpIH, BMPFileHeader* bmpFH, uint32_t* rSize, uint8_t* cArr, char* orientation) {
     uint8_t* newArr;
     swap(bmpIH->biHeight,bmpIH->biWidth);
     uint32_t newRowSize = (bmpIH->biBitCount * bmpIH->biWidth + 31) / 32 * 4;
     bmpIH->biSizeImage = newRowSize * bmpIH->biHeight;
     bmpFH->bfSize = bmpIH->biSizeImage + 54;
     if((newArr = malloc(bmpIH->biSizeImage)) == NULL) {
          printf("Invalid address");
          exit(-1);
     }
     uint32_t H = bmpIH->biHeight, W = bmpIH->biWidth;

     // From here on out the original width and height are swapped
     // Transpose the original color array
     for(uint32_t i = 0; i < H; i++)
          for(uint32_t j = 0; j < W; j++) {
               uint32_t offsetOr =  j*(*rSize) + i*3;
               uint32_t offsetNew = newRowSize * i + j*3;
               newArr[offsetNew+2] = cArr[offsetOr+2];
               newArr[offsetNew+1] = cArr[offsetOr+1];
               newArr[offsetNew] = cArr[offsetOr];
          }

     // If the image has to be flipped ccw, flip horizontally. Otherwise vertically
     if(strcmp(orientation, "ccw") == 0) {
          newArr = hFlip(H, newRowSize, W, newArr);
     } else {
          newArr = vFlip(H, newRowSize, newArr);
     }
     free(cArr);
     rSize = &newRowSize;
     return newArr;
}


// Rotate an image by first flipping the image horizontally, then vertically
uint8_t* rotateBMP180(BMPInfoHeader bmpIH, uint8_t* cArr, uint32_t rowSize) {
     uint32_t H = bmpIH.biHeight, W = bmpIH.biWidth;

     // Horizontally flip the image
     cArr = hFlip(H, rowSize, W, cArr);

     // Vertically flip the image
     cArr = vFlip(H, rowSize, cArr);
     return cArr;
}

// This function applies a sepia filter to the bmp image
void sepia(uint8_t* cArr, BMPInfoHeader bmpInfo, uint32_t rSize) {
     for(uint32_t i = 0; i<bmpInfo.biHeight; i++) {
          for(uint32_t j = 0; j<bmpInfo.biWidth*3; j+=3) {
               uint32_t offset = rSize * i + j;
               uint8_t tempBlue = cArr[offset];
               uint8_t tempGreen = cArr[offset+1];
               uint8_t tempRed = cArr[offset+2];
               cArr[offset+2] = tempRed * 0.393 + tempGreen * 0.769 + tempBlue * 0.189;
               cArr[offset+1] = tempRed * 0.349 + tempGreen * 0.686 + tempBlue * 0.118;
               cArr[offset] = tempRed * 0.272 + tempGreen* 0.534 + tempBlue * 0.131;
               cArr[offset+2] = min(cArr[offset+2], 255);
               cArr[offset+1] = min(cArr[offset+1], 255);
               cArr[offset] = min(cArr[offset],255);
          }
     }

}


// Function to write the bmp to a file
void writeBMP(FILE* fPtr, BMPFileHeader* bmpFH, BMPInfoHeader* bmpIH, uint8_t* CArr) {
     fseek(fPtr, 0, SEEK_SET);
     fwrite(bmpFH, sizeof(*bmpFH), 1, fPtr);
     fwrite(bmpIH, sizeof(*bmpIH), 1, fPtr);
     fwrite(CArr, bmpIH->biSizeImage, 1, fPtr);
     fflush(fPtr);
}
