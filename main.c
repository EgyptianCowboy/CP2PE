#include "bmpFunc.h"
#include "cmd.h"

int main() {

     // Init the bmp structs
     BMPFileHeader bmpFile = { 0, 0, 0, 0 , 0};
     BMPInfoHeader bmpInfo = { 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0,
                               0 };

     clearUni();
     FILE* bmpPtr = NULL;
     uint32_t rowSize = 0;

     // Init global color array pointer
     uint8_t* colorArray = NULL;

     // Init dirty bit
     uint8_t cFlag = 1;

     // Command line length
     char line[80] = {0};
     const char* delim = " \n";
     char* cmdToken;
     wMsg();

     while(cFlag) {
          fputs("$ ", stdout);

          // Read input line
          if(fgets(line, sizeof(line), stdin)) {
               // Tokenize the commands
               cmdToken = strtok(line, delim);

               // Giant if-else step to process the commands
               if(strcmp(cmdToken, "h") == 0 || strcmp(cmdToken, "help") == 0) {
                    hMsg();
               } else if(strcmp(cmdToken, "re") == 0 || strcmp(cmdToken, "read") == 0) {
                    printf("Reading image\n");
                    cmdToken = strtok(NULL, delim);
                    if((bmpPtr = fopen(cmdToken, "rb")) == NULL) {
                         printf("Invalid filename\n");
                         continue;
                    } else {
                         verifyBMP(bmpPtr, &bmpFile, &bmpInfo);
                         colorArray = malloc(bmpInfo.biSizeImage);
                         colorArray = toCArr(bmpPtr, bmpInfo, colorArray);
                         rowSize = (bmpInfo.biBitCount * bmpInfo.biWidth + 31) / 32 * 4;
                    }
               } else if(strcmp(cmdToken, "w") == 0 || strcmp(cmdToken, "write") == 0) {
                    if(!bmpPtr) {
                         printf("Initialize and image first\n");
                         continue;
                    } else {
                         cmdToken = strtok(NULL, delim);
                         if(!cmdToken) {
                              printf("Invalid filename\n");
                              continue;
                         }
                         FILE* tempPtr = fopen(cmdToken, "w");
                         writeBMP(tempPtr, &bmpFile, &bmpInfo, colorArray);
                         fclose(tempPtr);
                    }

               } else if(strcmp(cmdToken, "e") == 0 || strcmp(cmdToken, "exit") == 0) {
                    printf("Exiting\n");
                    if(bmpPtr) {
                         fclose(bmpPtr);
                         free(colorArray);
                    }
                    remove(TEMPIMAGE);
                    return EXIT_SUCCESS;
               } else if(strcmp(cmdToken, "d") == 0 || strcmp(cmdToken, "display") == 0) {
                    if(bmpPtr) {
                         FILE* tempPtr = fopen(TEMPIMAGE, "w");
                         writeBMP(tempPtr, &bmpFile, &bmpInfo, colorArray);
                         dispBMP(TEMPIMAGE);
                         fclose(tempPtr);
                    }else {
                         printf("Initialize an image first\n");
                         continue;
                    }
               } else if(strcmp(cmdToken, "i") == 0 || strcmp(cmdToken, "invert") == 0) {
                    if(!bmpPtr) {
                         printf("Initialize an image first\n");
                         continue;
                    }
                    printf("Inverting BMP\n");
                    invBMP(bmpInfo, colorArray);
               } else if(strcmp(cmdToken, "s") == 0 || strcmp(cmdToken, "sepa") == 0) {
                    if(!bmpPtr) {
                         printf("Initialize an image first\n");
                         continue;
                    }
                    printf("Applying sepia filter\n");
                    sepia(colorArray, bmpInfo, rowSize);
               } else if(strcmp(cmdToken, "ro") == 0 || strcmp(cmdToken, "rotate")) {
                    cmdToken = strtok(NULL, delim);
                    if(!bmpPtr) {
                         printf("Initialize an image first\n");
                         continue;
                    } else if(strcmp(cmdToken, "180") == 0 || strcmp(cmdToken, "-180") == 0) {
                         colorArray = rotateBMP180(bmpInfo, colorArray, rowSize); }
                    else if(strcmp(cmdToken, "90") == 0) {
                         cmdToken = strtok(NULL, delim);
                         if(strcmp(cmdToken, "cw") == 0) {
                              colorArray = rotateBMP90(&bmpInfo, &bmpFile,& rowSize, colorArray, cmdToken);
                         } else if(strcmp(cmdToken, "ccw") == 0) {
                                   colorArray = rotateBMP90(&bmpInfo, &bmpFile,& rowSize, colorArray, cmdToken);
                         } else {
                              printf("Invalid command\n");
                              continue;
                         }
                    } else printf("Invalid command\n");
               } else printf("Invalid command\n");
          }
     }
     return EXIT_SUCCESS;
}
