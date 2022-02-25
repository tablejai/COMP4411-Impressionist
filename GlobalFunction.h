#ifndef GLOBAL_FUNC
#define  GLOBAL_FUNC
#include "ImpBrush.h"

double meanfilter(const Point source, unsigned char* data, int width, int height);
double meanfilter(const Point source, unsigned char* data, int width, int height,int incre);

double getGradientX(const Point source, unsigned char* data, int width, int height);
double getGradientY(const Point source, unsigned char* data, int width, int height);

#endif