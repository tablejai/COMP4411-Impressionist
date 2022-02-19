//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include <iostream>
#include <random>
#include  <unordered_map>
using namespace std;




#define LEFT_MOUSE_DOWN		1

#define LEFT_MOUSE_DRAG		2

#define LEFT_MOUSE_UP		3

#define RIGHT_MOUSE_DOWN	4

#define RIGHT_MOUSE_DRAG	5

#define RIGHT_MOUSE_UP		6



#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6
#define AUTO_PAINT          7


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
Point	coord;
Point	oldcoord;
Point mouseVec;


PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{


	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	initPaint = false;
	gradientMode = DEFUALT;
   

}
void PaintView::INIT_RGBA(GLvoid* data, unsigned char*& RGBA, int w, int h, int a) {

	if (w > 0 && h > 0) {
		RGBA = new unsigned char[w * h * 4];
		memset(RGBA, 0, w * h * 4);
		for (int j = 0;j < h;j++) {
			int id = 0;
			for (int i = 0; i < w * 4;i++) {
				int index = i % 4;
				if (index < 3)
					Map4(RGBA, i, j, w) = Map(data, id, j, w);
				else {
					Map4(RGBA, i, j, w) = (char)a;
					id--;
				}
				id++;
			}
		}
	}
	cout << w << "," << h << endl;
	cout << "rgb to rgba" << endl;

}



void PaintView::randomBrushDraw() {
	
	



}

GradientMode PaintView::getGradientMode() {
	return  gradientMode;
}
void PaintView::setGradientMode(GradientMode mode) {
	gradientMode = mode;
}
void PaintView::autoPaint() {
	glDrawBuffer(GL_BACK);
	auto seed = mt19937{ random_device()() };
	mt19937 mt(seed);
	int length;
	m_pDoc->clearCanvas();
	SynchronizeContentRGBA(rgbaBrush, m_pUndoBitstart);
	RestorePreviousDataRGBA(rgbaBrush, GL_BACK);
	vector<Point> points;
	m_pDoc->m_pCurrentBrush->setBrushMode(RANDOMMODE);
	for (int i = 0;i < m_nDrawHeight;i++) {
		for (int j = 0;j < m_nDrawWidth;j++) {
			points.push_back(Point{j,i});
		}
	}
	random_shuffle(points.begin(), points.end());
	for(int k=0;k<points.size();k++){
		    int i = points[k].y, j = points[k].x;
			Point source(j + m_nStartCol, m_nEndRow - i);
			Point target(j, m_nWindowHeight - i);
			coord.x = j;
			coord.y = i;
			m_pDoc->m_pCurrentBrush->BrushBegin(source, target);//should draw to back
			m_pDoc->m_pCurrentBrush->BrushMove(source, target);//should draw to back
			m_pDoc->m_pCurrentBrush->BrushEnd(source, target);//should draw to back
	}
	std::cout << m_pDoc->m_pCurrentBrush->BrushName()<<"bursh nm" << endl;
	SavePreviousDataRGBA(rgbaBrush, GL_BACK);
	clearColorBuffer(GL_BACK);
	AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
	AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
	glFlush();
	glDrawBuffer(GL_BACK);
	this->redraw();
	m_pDoc->m_pCurrentBrush->setBrushMode(NORMALMODE);

}



void PaintView::draw()
{
#ifndef MESA
		// To avoid flicker on some machines.
		glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA

		if (!valid())
		{

			glClearColor(0.7f, 0.7f, 0.7f, 1.0);

			// We're only using 2-D, so turn off depth 
			glDisable(GL_DEPTH_TEST);

			ortho();

			glClear(GL_COLOR_BUFFER_BIT);
		}
		Point scrollpos;// = GetScrollPosition();
		scrollpos.x = 0;
		scrollpos.y = 0;

		m_nWindowWidth = w();
		m_nWindowHeight = h();

		int drawWidth, drawHeight;
		drawWidth = min(m_nWindowWidth, m_pDoc->m_nPaintWidth);
		drawHeight = min(m_nWindowHeight, m_pDoc->m_nPaintHeight);

		int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
		if (startrow < 0) startrow = 0;

		m_pPaintBitstart = m_pDoc->m_ucPainting +
			3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);
		m_pUndoBitstart = m_pDoc->m_undoBitMap + 4 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

		m_nDrawWidth = drawWidth;
		m_nDrawHeight = drawHeight;
		m_nStartRow = startrow;
		m_nEndRow = startrow + drawHeight;
		m_nStartCol = scrollpos.x;
		m_nEndCol = m_nStartCol + drawWidth;

		if (!rgbaBitMap&& m_pDoc->m_rgbaBitMap) { //init load new image 
			rgbaBitMap = m_pDoc->m_rgbaBitMap + 4 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);
			clearColorBuffer(GL_BACK);
			RGB_TO_RGBA(m_pDoc->m_ucBitmap, rgbaBitMap, m_pDoc->m_nWidth, m_pDoc->m_nHeight, backgroundalpha * 255);
			AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
		}
		else if(rgbaBitMap)
			RGB_TO_RGBA(m_pDoc->m_ucBitmap, rgbaBitMap, m_pDoc->m_nWidth, m_pDoc->m_nHeight, backgroundalpha * 255);
		if (!rgbaBrush && m_pDoc->m_rgbaBrush) {
			rgbaBrush = m_pDoc->m_rgbaBrush + 4 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);
		}
		if (m_pDoc->m_ucPainting && m_pDoc->m_rgbaBitMap && m_pDoc->m_rgbaBrush && !isAnEvent)
		{
			refreshPaintView();
		}
		if (m_pDoc->m_ucPainting && m_pDoc->m_rgbaBitMap && m_pDoc->m_rgbaBrush && isAnEvent)
		{

			isAnEvent = 0;
			Point source(coord.x + m_nStartCol, m_nEndRow - coord.y);
			Point target(coord.x, m_nWindowHeight - coord.y);
			switch (eventToDo)
			{
			case LEFT_MOUSE_DOWN:
				SynchronizeContentRGBA(rgbaBrush, m_pUndoBitstart);
			//	clearColorBuffer(GL_BACK);
				RestorePreviousDataRGBA(rgbaBrush,GL_BACK);
				if (source.y < 0 || source.x > m_nWindowWidth) return;
				m_pDoc->m_pCurrentBrush->BrushBegin(source, target);//should draw to back
				SavePreviousDataRGBA(rgbaBrush, GL_BACK);
				clearColorBuffer(GL_BACK);
				AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
				AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
				break;
			case LEFT_MOUSE_DRAG:
				if (source.y < 0 || source.x > m_nWindowWidth) return;
				//clearColorBuffer(GL_BACK);
				//AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
				RestorePreviousDataRGBA(rgbaBrush, GL_BACK);
				m_pDoc->m_pCurrentBrush->BrushMove(source, target);
				SavePreviousDataRGBA(rgbaBrush, GL_BACK);
				clearColorBuffer(GL_BACK);
				AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
				AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);

				break;
			case LEFT_MOUSE_UP:
				//clearColorBuffer(GL_BACK);
				//AddPreviousDataRGBA(rgbaBrush, GL_BACK);
				RestorePreviousDataRGBA(rgbaBrush, GL_BACK);
				m_pDoc->m_pCurrentBrush->BrushEnd(source, target);
				SavePreviousDataRGBA(rgbaBrush, GL_BACK);
				clearColorBuffer(GL_BACK);
				AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
				AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);

				break;

			case RIGHT_MOUSE_DOWN:

				break;
			case RIGHT_MOUSE_DRAG:

				break;
			case RIGHT_MOUSE_UP:

				break;
			case AUTO_PAINT:


				break;
			default:
				printf("Unknown event!!\n");
				break;
			}
		}
		if (drawstate == UNDO) {
			//clearColorBuffer(GL_BACK);
			SynchronizeContentRGBA(m_pUndoBitstart, rgbaBrush);
			clearColorBuffer(GL_BACK);
			AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
			AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
			drawstate = DO;
		}

		glFlush();

#ifndef MESA
		// To avoid flicker on some machines.
		glDrawBuffer(GL_BACK);
#endif // !MESA

}

void PaintView::refreshPaintView() {
	//clearColorBuffer(GL_BACK);
	RestorePreviousDataRGBA(rgbaBrush, GL_BACK);
	SavePreviousDataRGBA(rgbaBrush, GL_BACK);
	clearColorBuffer(GL_BACK);
	AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
	AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
}

void PaintView::resetBackGround(unsigned char* ptr) {
	rgbaBitMap = ptr;
}
void PaintView::resetBrush(unsigned char* ptr) {
	rgbaBrush = ptr;
}
void PaintView::resetUndo(unsigned char* ptr) {
	m_pUndoBitstart = ptr;
}
void PaintView::updateBackGroundAlpha(float alpha) {
	backgroundalpha = alpha;
}


int PaintView::handle(int event)
{
if(m_pDoc->m_pCurrentBrush->bmode==NORMALMODE)
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		if (abs(coord.x - oldcoord.x) > 0 && abs(coord.y - oldcoord.y) > 0)
		{
			mouseVec.x = coord.x - oldcoord.x;
			mouseVec.y = coord.y - oldcoord.y;
			oldcoord = coord;

		}		
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		m_pDoc->m_pCursor->setpos(coord.x, coord.y);
		m_pDoc->m_pUI->m_origView->triggerupdate();
		redraw();
		if (abs(coord.x - oldcoord.x) >0 && abs(coord.y - oldcoord.y) > 0)
		{
			mouseVec.x = coord.x - oldcoord.x;
			mouseVec.y = coord.y - oldcoord.y;
			oldcoord = coord;

		}
		break;
	case FL_DRAG: {
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_DRAG;
		else
			eventToDo = LEFT_MOUSE_DRAG;
		isAnEvent = 1;
		if (coord.x > 0 && coord.x <= m_nWindowWidth && coord.y > 0 && coord.y <= m_nWindowHeight)
			m_pDoc->m_pCursor->setpos(coord.x, coord.y);
	
		m_pDoc->m_pUI->m_origView->triggerupdate();
		redraw();
		if (abs(coord.x - oldcoord.x) > 0 && abs(coord.y - oldcoord.y) > 0)
		{
			mouseVec.x = coord.x - oldcoord.x;
			mouseVec.y = coord.y - oldcoord.y;
			oldcoord = coord;

		}

		break;
	}
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		m_pDoc->m_pCursor->setpos(coord.x, coord.y);

		m_pDoc->m_pUI->m_origView->triggerupdate();
		redraw();
		if (abs(coord.x - oldcoord.x) > 0 && abs(coord.y - oldcoord.y) > 0)
		{
			mouseVec.x = coord.x - oldcoord.x;
			mouseVec.y = coord.y - oldcoord.y;
			oldcoord = coord;

		}

		break;

	case FL_MOVE: {

		coord.x = Fl::event_x();

		coord.y = Fl::event_y();

		if (coord.x > 0 && coord.x <= m_nWindowWidth && coord.y > 0 && coord.y <= m_nWindowHeight)

			m_pDoc->m_pCursor->setpos(coord.x, coord.y);
		/*DEBUG start*/
		Point target(coord.x, m_nWindowHeight - coord.y);
		glReadBuffer(GL_FRONT);

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
		unsigned char b[3];
		glReadPixels(target.x,
			target.y,
			1,
			1,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			b);
		cout << (int)b[0] << "," << (int)b[1] << "," << (int)b[2] << "|" << endl;
		/*DEBUG end*/
		m_pDoc->m_pUI->m_origView->triggerupdate();
		if (abs(coord.x - oldcoord.x) > 1 && abs(coord.y - oldcoord.y) > 1)
		{
			mouseVec.x = coord.x - oldcoord.x;
			mouseVec.y = coord.y - oldcoord.y;
			oldcoord = coord;

		}
		break;}
	default:
		return 0;
		break;

	}


	return 1;
}


void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}
void PaintView::undo() {
	if (m_pUndoBitstart != nullptr) {
		cout << "undo bit" << endl;
		drawstate = UNDO;
		redraw();
		return;
	}
	//cout << "it did not return " << endl;

}
//void PaintView::clearView() {
//	drawstate = CLEAR;
//	m_pPaintBitstart = nullptr;
//	m_pUndoBitstart = nullptr;
//	rgbaBitMap = nullptr;
//	rgbaBrush = nullptr;
//	redraw();
//}
void PaintView::setstate(ReDrawState state) {
	drawstate = state;
}
void PaintView::SaveCurrentContent(GLenum mode)
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(mode);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
			  m_nDrawWidth, 
			  m_nDrawHeight, 
			  GL_RGB, 
			  GL_UNSIGNED_BYTE,
		  m_pPaintBitstart);

}

void PaintView::SavePreviousData(GLvoid* data) {
	if (data == nullptr) {
		cout << "isnull" << endl;
		return;
	}
	cout << "save previous data" << endl;
	glReadBuffer(GL_FRONT);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
	glReadPixels(0,
		m_nWindowHeight - m_nDrawHeight,
		m_nDrawWidth,
		m_nDrawHeight,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		data);

}
void PaintView::SavePreviousDataRGBA(GLvoid* data,GLenum mode) {
	if (data == nullptr) {
		cout << "isnull" << endl;
		return;
	}
	//cout << "save previous data [RGBA]" << endl;
	glReadBuffer(mode);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
	glReadPixels(0,
		m_nWindowHeight - m_nDrawHeight,
		m_nDrawWidth,
		m_nDrawHeight,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data);

}

void PaintView::RestorePreviousData(GLvoid* data) {
	glDrawBuffer(GL_BACK);
//	cout << "restore previous data" << endl;
	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2i(0, m_nWindowHeight - m_nDrawHeight);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
	glDrawPixels(m_nDrawWidth,
		m_nDrawHeight,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		data);
}





void PaintView::RestorePreviousDataRGBA(GLvoid* data,GLenum mode) {
	glDrawBuffer(GL_BACK);
	//	cout << "restore previous data" << endl;
	//glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2i(0, m_nWindowHeight - m_nDrawHeight);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
	glDrawPixels(m_nDrawWidth,
		m_nDrawHeight,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data);
}


void PaintView::AddPreviousDataRGBA(GLvoid* data, GLenum mode, PaintMode ptMode) {
	glDrawBuffer(mode);
	if(data!=nullptr)
	if (ptMode == COVER) {
		glRasterPos2i(0, m_nWindowHeight - m_nDrawHeight);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
		glDrawPixels(m_nDrawWidth,
			m_nDrawHeight,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data);
	}
	else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPointSize(1);
		glBegin(GL_POINTS);
		for (int j = 0;j < m_pDoc->m_nPaintHeight;j++) {
			for (int i = 0;i < m_pDoc->m_nPaintWidth;i++) {
			GLbyte r = Map4(data, i*4, j, m_pDoc->m_nPaintWidth);
			GLbyte g = Map4(data, i * 4+1, j, m_pDoc->m_nPaintWidth);
			GLbyte b = Map4(data, i * 4 + 2, j, m_pDoc->m_nPaintWidth);
			GLbyte a = Map4(data, i * 4 + 3, j, m_pDoc->m_nPaintWidth);
			glColor4ub(r,g,b,a);
			glVertex2d(i,j);
			}
		}
		glEnd();
		glDisable(GL_BLEND);

	}
}
void PaintView::clearColorBuffer( GLenum buffer) {
	glDrawBuffer(buffer);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0,0,0,0); //dont put 0
	glClear(GL_COLOR_BUFFER_BIT);
	//glDisable(GL_BLEND);


}

void PaintView::RestoreContent(GLenum mode)
{
	glDrawBuffer(mode);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);
}
void PaintView::SynchronizeContent(GLvoid* source, GLvoid* target) {
for (int i = 0;i < m_pDoc->m_nPaintWidth*3;i++) {
	for (int j = 0;j < m_pDoc->m_nPaintHeight;j++) {
		Map(target,i,j,m_pDoc->m_nPaintWidth) = Map(source,i,j,m_pDoc->m_nPaintWidth);
	}
}
}
void PaintView::SynchronizeContentRGBA(GLvoid* source, GLvoid* target) {
	for (int i = 0;i < m_pDoc->m_nPaintWidth * 4;i++) {
		for (int j = 0;j < m_pDoc->m_nPaintHeight;j++) {
			Map4(target, i, j, m_pDoc->m_nPaintWidth) = Map4(source, i, j, m_pDoc->m_nPaintWidth);
		}
	}
}
