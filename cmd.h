#ifndef CMD_HDR
#define CMD_HDR
#include<stdlib.h>
#define FILENAME "image.bmp"
#define FILENAMENEW "image2.bmp"
#define SYSIMAGE "display -resize '200%' " FILENAME "&"
#define SYSIMAGENEW "display -resize '200%' " FILENAMENEW "&"

void clearUni();
void dispBMP();

#endif
