//
// paintview.h
//
// The header file for painting view of the input images
//

#ifndef PAINTVIEW_H
#define PAINTVIEW_H
#include "BrushStroke.h"
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <stdlib.h>
#include <vector>
using namespace std;
class ImpressionistDoc;
extern Point    coord;
extern Point	oldcoord;
extern Point    mouseVec;
typedef enum {
UNDO,
DO
} ReDrawState;
typedef enum {
NONCOVER,
COVER
} PaintMode;
typedef enum {
DEFUALT,
CUSTOM
} GradientMode;

typedef enum {
Impressionist,
Expressionist,
Pointillist, 
Psychedelic,
Custom,
NumOfPaintly
}PaintlyMode;
typedef struct {
	PaintlyMode mode;
	float blurrSize;
	float fc;
	float fg;
	float a ;
	float threshold;
	float jr;
	float jg;
	float jb;
	int maxStrokeLength;
	int minStrokeLength;

}PaintParameter;
extern PaintParameter paintModes[4];

class PaintView : public Fl_Gl_Window
{



public:
	PaintView(int x, int y, int w, int h, const char *l);
	void draw();
	int handle(int event);
	void drawPaintly();
	void updatePaintlyPara();
	void refresh();
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
	void randomBrushDraw();
	void clearColorBuffer(GLenum);
	void resetBackGround(unsigned char* ptr=nullptr);
	void resetBrush(unsigned char* ptr=nullptr);
	void resetUndo(unsigned char* ptr = nullptr);
	GradientMode getGradientMode();
	void setGradientMode(GradientMode);
	void autoPaint();
	void Paint(bool );
	void updateBackGroundAlpha(float);
	void blurr(unsigned char* , unsigned char* ,float);
	double vectorDistance(const vector<int>& v1, const vector<int>& v2);
	void INIT_RGBA(GLvoid* , unsigned char*&  , int , int , int );
	void blurrPoint(unsigned char* sourceptr,unsigned char *ptr, const Point source,const Point target, int paintWidth, int paintHeight, const vector<vector<double>>& kernel);
	void blurrPointOrigin(unsigned char* ptr, const Point source, const Point target, int paintWidth, int paintHeight, const vector<vector<double>>& kernel);

	void KernelSetColor(unsigned char* ptr, const Point target, const int r, const int g, const int b, const int alpha);
	int maxDistance(vector<int>);
	BrushStroke PaintStroke(int targetx, int targety, unsigned char* canvas,int w, int h ,int radius, unsigned char* target,float kernel);
	unsigned char* getrgbaBrush();
	//void RGB_TO_RGBA(GLvoid* , unsigned char*, int , int , int);
	void undo();
	ImpressionistDoc *m_pDoc;
	bool initPaint;
	unsigned char* rgbaBrush = nullptr;
	unsigned char* rgbaBitMap = nullptr;
	float blurrSize = 0.5f;
	float fc = 0.25f;
	float fg = 1;
	float a = 0.7;
	float threshold = 50;
	int maxStrokeLength = 16;
	int minStrokeLength = 10;
	PaintlyMode paintlymode;
private:
	GLvoid *m_pPaintBitstart;
//	GLvoid* m_pBrushstart;
	float backgroundalpha;

	GLvoid* m_pImageShawdow;
	GLvoid* m_pUndoBitstart;
	ReDrawState drawstate;
	GradientMode gradientMode;
	std::vector<int> brushRadii;
public:
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