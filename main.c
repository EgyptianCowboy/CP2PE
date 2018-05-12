#include "bmpFunc.h"
#include "cmd.h"

int main() {
  BMPFileHeader bmpFile = { 0, 0, 0, 0, 0 };
  BMPInfoHeader bmpInfo = { 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0,
                            0 };

  //rgbQuad bmpC = { 0, 0, 0 };
  rgbQuad* colorArray = NULL;
  printf("reading image\n");
  FILE* bmpPtr = fopen(FILENAME, "rb");
  clearUni();

  verifyBMP(bmpPtr, &bmpFile, &bmpInfo);
  uint32_t width = bmpInfo.biWidth;
  uint32_t height = bmpInfo.biHeight;
  uint32_t pixels = width*height;
  colorArray = toCArr(bmpPtr, pixels);
  invBMP(pixels, colorArray);
  //colorArray = rotateBMP(&height, &width, colorArray);
  //colorArray = rotateBMP(&height, &width, colorArray);
  //colorArray = rotateBMP(&height, &width, colorArray);
  fclose(bmpPtr);
  FILE* bmpPtrNew = fopen(FILENAMENEW, "w");
  writeBMP(bmpPtrNew, &bmpFile, &bmpInfo, colorArray, pixels);
  fclose(bmpPtrNew);
  free(colorArray);
  dispBMP();

  return EXIT_SUCCESS;
}
