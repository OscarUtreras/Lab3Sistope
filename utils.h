#include "bmp_format.h"

#ifndef UTILS_H
#define UTILS_H

char **buildArguments(char* file, int i, int umbBinary, int umbClassi, int flag, int fd);
void sendHeadBMP(int fd[], BMP *img);
BMP *catchHeadBMP(int fd);
void printBMP(BMP *img);
void compare(BMP *img1, BMP *img2);
void sendMatrixBMP(int fd, BMP* img);
void catchMatrixBMP(int fd, BMP *img);

#endif