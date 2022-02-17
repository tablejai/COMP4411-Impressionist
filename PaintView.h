//
// paintview.h
//
// The header file for painting view of the input images
//

#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <stdlib.h>

class ImpressionistDoc;
extern Point    coord;
extern Point	oldcoord;
extern Point    mouseVec;
typedef enum {
UNDO,
CLEAR,
LOAD,
BLEND,
DO
} ReDrawState;
typedef enum {
NONCOVER,
COVER
} PaintMode;
class PaintView : public Fl_Gl_Window
{
public:
	PaintView(int x, int y, int w, int h, const char *l);
	void draw();
	int handle(int event);

	void refresh();
	//void clearView();
	void resizeWindow(int width, int height);
	void refreshPaintView();
	void setstate(ReDrawState);
	void SaveCurrentContent(GLenum mode);
	void SavePreviousDataRGBA(GLvoid*, GLenum);
	void SavePreviousData(GLvoid*);
	void RestorePreviousDataRGBA(GLvoid* ,GLenum);
	void AddPreviousDataRGBA(GLvoid*, GLenum, PaintMode);
	void SynchronizeContent(GLvoid*, GLvoid*);
	void SynchronizeContentRGBA(GLvoid*, GLvoid*);
	void RestorePreviousData(GLvoid*);
	void RestoreContent(GLenum mode);
	void clearColorBuffer(GLenum);
	void resetBackGround();
	void resetBrush();
	void updateBackGroundAlpha(float);
	void INIT_RGBA(GLvoid* , unsigned char*&  , int , int , int );
	void RGB_TO_RGBA(GLvoid* , unsigned char*, int , int , int);
	void RGBA_TO_RGB(GLvoid*, unsigned char*, int, int, int);
	void undo();
	ImpressionistDoc *m_pDoc;
	bool initPaint;

private:
	GLvoid *m_pPaintBitstart;
//	GLvoid* m_pBrushstart;
	unsigned char* rgbaBitMap=nullptr;
	unsigned char* rgbaBrush = nullptr;
	float backgroundalpha;
	GLvoid* m_pImageShawdow;
	GLvoid* m_pUndoBitstart;
	ReDrawState drawstate;
	int m_nDrawWidth,
		m_nDrawHeight,
		m_nStartRow,
		m_nEndRow,
		m_nStartCol,
		m_nEndCol,
		m_nWindowWidth,
		m_nWindowHeight;
};


#endif