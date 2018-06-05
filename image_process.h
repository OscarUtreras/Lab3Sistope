#include "bmp_format.h"

#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

void GreyScale(BMP* image);
void Binary(BMP*image, int umbBinary);
int nearlyBlack(BMP *image, int umbCla);

#endif
