#include "bmp_format.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#ifndef IO_IMAGE_H
#define IO_IMAGE_H

BMP *LoadBMP(char *filename);
void SaveBMP(BMP *imagen, char *filename);

#endif
