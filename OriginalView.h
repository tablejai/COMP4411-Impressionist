//
// originalview.h
//
// The header file for original view of the input images
//

#ifndef ORIGINALVIEW_H
#define ORIGINALVIEW_H

#include <FL/Fl.H>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include <stdlib.h>

#include "ImpBrush.h"

class ImpressionistDoc;
typedef enum {
 NORMAL_VIEW,
 BLEND_VIEW
}Blend;

class OriginalView : public Fl_Gl_Window
{
static int		eventToDo;
static int		isAnEvent;
static Point	coord;
public:
	OriginalView(int x, int y, int w, int h, const char *l);

	void draw();
	void refresh();
	void resizeWindow(int width, int height);
	void triggerupdate();
	int getHeight();
	int getWidth();
	void blendImage();

	Blend state;
	ImpressionistDoc *m_pDoc;

private:
	int m_nWindowWidth,
		m_nWindowHeight;
};

#endif