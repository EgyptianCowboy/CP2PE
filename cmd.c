#include "cmd.h"

// Function to clear the terminal window
void clearUni() {
#ifdef _WIN32
     system("cls");
#else
     system("clear");
#endif
}

// Function to display the image in linux
void dispBMP (char* filename) {
#ifdef __linux__
     uint8_t namelen = strlen(filename);
     char* dispBuf = malloc(sizeof(char)*(namelen+10));
     snprintf(dispBuf, namelen + 20, "display %s &", filename);
     system(dispBuf);
     free(dispBuf);
#endif
}

// The welcoming message
void wMsg () {
     printf("This is a basic BMP manipulation program\n");
     printf("V0.0.1\n");
     printf("Author: Sil Vaes\n\n");
     printf("Type \"h\" or \"help\" to see a list of commands\n\n");
}


// The help message
void hMsg() {
     printf("Available commands are: \n\n");
     printf("\"read\" or \"re\" \t \t Read an image, arguments: filename.bmp\n");
     printf("\"write\" or \"w\" \t \t Write an image, arguments: filename.bmp\n");
     printf("\"display\" or \"d\" \t \t Display a bmp image image (only works with ImageMagick and depends on filepointer)n");
     printf("\"invert\" or \"i\" \t Invert the image colors\n");
     printf("\"rotate\" or \"ro\" \t Rotate the image with arguments: 90 or 180 and cw or ccw. Example r 90 cw\n");
     printf("\"sepia\" or \"s\" \t\t Apply a sepia filter to the image\n");
     printf("\"exit\" or \"e\" \t \t Exit the program\n");
}
