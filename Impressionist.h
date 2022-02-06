#ifndef IMPRESSIONIST_H
#define IMPRESSIONIST_H
#define SAMPLE_SOLUTION
#include <stdio.h>

#include <FL/Fl.h>
#include <FL/gl.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "ImpBrush.h"

#define TRUE 1
#define FALSE 0

#ifdef WIN32
#define M_PI 3.1415926535F
#endif

#endif