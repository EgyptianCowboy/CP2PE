#include "cmd.h"

void clearUni() {
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
    system(SYSIMAGE);
  #endif
}

void dispBMP () {
  #ifdef __linux__
    system(SYSIMAGENEW);
  #endif
}
