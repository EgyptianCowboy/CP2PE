#include "bmpFunc.h"

void swap(rgbQuad a, rgbQuad b) {
  rgbQuad temp = a;
  a = b;
  b = temp;
}

void verifyBMP(FILE* fPtr, BMPFileHeader* bmpFH, BMPInfoHeader* bmpIH) {
  //rgbQuad bmpQ;

  fread(bmpFH, sizeof(*bmpFH), 1, fPtr);
  fread(bmpIH, sizeof(*bmpIH), 1, fPtr);

  // read filetype
  //fread(&bmpFH->bfType, 2, 1, fPtr);
  printf("Filetype is %"PRIx16"\n", bmpFH->bfType);
  if(bmpFH->bfType != 0x4d42) {
    printf("Not a bitmap\n");
    exit(-1);
  }
  // filesize
  //fread(&bmpFH->bfSize, 4, 1, fPtr);
  printf("Filesize is %"PRIu32" bytes\n", bmpFH->bfSize);
  // move 4 bytes, to ignore reserved bits
  //fseek(fPtr, 4, SEEK_CUR);
  // offset
  //fread(&bmpFH->bfOffbits, 4, 1, fPtr);
  printf("Offset is %"PRIu32" bytes\n", bmpFH->bfOffbits);
  // infoheader
  //fread(&bmpIH->biSize, 4, 1, fPtr);
  printf("Info header size is %"PRIu32" bytes\n", bmpIH->biSize);
  // fileheader has to be 40 bytes in size
  if(bmpIH->biSize != 40) {
    printf("Not a bitmap\n");
    exit(-1);
  }
  // width and height
  //fread(&bmpIH->biWidth, 4, 1, fPtr);
  //fread(&bmpIH->biHeight, 4, 1, fPtr);
  printf("Width is %"PRIu32" and height is %"PRIu32"\n", bmpIH->biWidth,
         bmpIH->biHeight);
  // planes
  //fread(&bmpIH->biPlanes, 2, 1, fPtr);
  printf("Amount of color planes is %"PRIu16"\n", bmpIH->biPlanes);
  // bits per pixel
  //fread(&bmpIH->biBitCount, 2, 1, fPtr);
  printf("Bytes per pixel: %"PRIu16"\n", bmpIH->biBitCount);
  // compression
  //fread(&bmpIH->biCompression, 4, 1, fPtr);
  printf("Compression type is %"PRIu32"\n", bmpIH->biCompression);
  // bitmap size
  //fread(&bmpIH->biSizeImage, 4, 1, fPtr);
  printf("Bitmap size is %"PRIu32"\n", bmpIH->biSizeImage);
  // resolution
  //fread(&bmpIH->biXPelsPerMeter, 4, 1, fPtr);
  //fread(&bmpIH->biYPelsPerMeter, 4, 1, fPtr);
  printf("Resolution is %"PRIu32" by %"PRIu32"\n", bmpIH->biXPelsPerMeter,
         bmpIH->biYPelsPerMeter);
  // used colors
  //fread(&bmpIH->biClrUsed, 4, 1, fPtr);
  printf("Number of used colors are %"PRIu32"\n", bmpIH->biClrUsed);
  // important colors
  //fread(&bmpIH->biClrImportant, 4, 1, fPtr);
  printf("Numer of important colors are %"PRIu32"\n", bmpIH-> biClrImportant);
}

rgbQuad* toCArr(FILE* fPtr, uint32_t pixels) {
  rgbQuad* tempArr = malloc(sizeof(rgbQuad)*pixels);
  for(uint32_t i=0; i<pixels; i++)
    fread(&(tempArr[i]), sizeof(rgbQuad), 1, fPtr);
  return tempArr;
}

void invBMP(uint32_t pixels, rgbQuad* colors) {
  for(uint32_t i=0; i<pixels; i++) {
    (colors[i]).blue = ~(colors[i]).blue;
    (colors[i]).green = ~(colors[i]).green;
    (colors[i]).red = ~(colors[i]).red;
    }
}

void tpArr(uint32_t* height, uint32_t* width, rgbQuad** CArr, rgbQuad** newCArr) {
  for(uint8_t i=0; i<(*height); i++) {
    for(uint8_t j=0; j<(*width); j++) {
      (newCArr[j][i]).blue = (CArr[i][j]).blue;
      (newCArr[j][i]).green = (CArr[i][j]).green;
      (newCArr[j][i]).red = (CArr[i][j]).red;
    }}
}

void reverseRow(uint32_t* height, uint32_t* width, rgbQuad** CArr) {
  rgbQuad temp = { 0, 0, 0, 0};
  for(uint32_t i=0; i<(uint32_t)floor((*width)/2); i++) {
    for(uint32_t j=0; j<*height; j++) {
      temp = CArr[(*width)-i-1][j];
      CArr[(*width)-i-1][j] = CArr[i][j];
      CArr[i][j] = temp;
      }}
  /*rgbQuad temp = { 0, 0, 0 };
  for(uint32_t i=0; i<*width; i++) {
    for(uint32_t j=0; j<((uint32_t)floor(*height/2)); j++) {
      temp = CArr[i][(*height)-j-1];
      CArr[i][(*height)-j-1] = CArr[i][j];
      CArr[i][j] = temp;
      }}*/
}

rgbQuad** rotateBMP(uint32_t* height, uint32_t* width, rgbQuad** CArr) {
  rgbQuad** tempArr = malloc(sizeof(rgbQuad*)*(*height));
  //**tempArr = {0};
  for(uint8_t i=0; i<(*width); i++) {
    if((tempArr[i] = malloc(sizeof(rgbQuad)*(*height))) == NULL) {
      printf("Invalid address");
      exit(-1);
    }}
  tpArr(height, width, CArr, tempArr);
  uint32_t tempDim = *width;
  *width = *height;
  *height = tempDim;
  reverseRow(height, width,  tempArr);
  free(CArr);
  return tempArr;
}

void writeBMP(FILE* fPtr, BMPFileHeader* bmpFH, BMPInfoHeader* bmpIH, rgbQuad* CArr, uint32_t pixels) {
  fseek(fPtr, 0, SEEK_SET);
  fwrite(bmpFH, sizeof(*bmpFH), 1, fPtr);
  fwrite(bmpIH, sizeof(*bmpIH), 1, fPtr);
  #ifdef DEBUG
    printf("bfSize is %"PRIu32"\n", bmpFH->bfSize);
    printf("biSize is %"PRIu32"\n", bmpIH->biSize);
    printf("Width is %"PRIu32" and height is %"PRIu32"\n", bmpIH->biWidth, bmpIH->biHeight);
    printf("Size of rgbquad %lu \n", sizeof(rgbQuad));
  #endif

    //for(uint8_t i = 0; i < ((width)*(height)); i++)
    //fwrite(&CArr[i], sizeof(rgbQuad), 1, fPtr);
    for(uint32_t i=0; i < pixels; i++) {
      fwrite(&CArr[i], sizeof(rgbQuad), 1, fPtr);
      fflush(fPtr);
    }
}
