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

}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);
	m_pUndoBitstart = m_pDoc->m_undoBitMap + 3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();

	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );

		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			SavePreviousData(m_pUndoBitstart);
			if (source.y < 0 || source.x > m_nWindowWidth) return;
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target);
			break;
		case LEFT_MOUSE_DRAG:
			if (source.y < 0 || source.x > m_nWindowWidth) return;
			m_pDoc->m_pCurrentBrush->BrushMove( source, target);
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target);
			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:

			break;
		case RIGHT_MOUSE_DRAG:

			break;
		case RIGHT_MOUSE_UP:

			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}
	}
	if (drawstate==UNDO) {
		RestorePreviousData(m_pUndoBitstart);
		SynchronizeContent(m_pUndoBitstart, m_pPaintBitstart);
		drawstate = DO;
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
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
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
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

	case FL_MOVE:

		coord.x = Fl::event_x();

		coord.y = Fl::event_y();

		if(coord.x>0 && coord.x<= m_nWindowWidth && coord.y>0 && coord.y<= m_nWindowHeight)

			m_pDoc->m_pCursor->setpos(coord.x,coord.y);

		m_pDoc->m_pUI->m_origView->triggerupdate();	
		if (abs(coord.x - oldcoord.x) > 1 && abs(coord.y - oldcoord.y) > 1)
		{
			mouseVec.x = coord.x - oldcoord.x;
			mouseVec.y = coord.y - oldcoord.y;
			oldcoord = coord;

		}

		break;
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
void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

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

void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

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
#define Map(A,i,j)  *((char*)A+(i+ 3 * (m_pDoc->m_nPaintWidth)*j))
void PaintView::SynchronizeContent(GLvoid* source, GLvoid* target) {
for (int i = 0;i < m_pDoc->m_nPaintWidth*3;i++) {
	for (int j = 0;j < m_pDoc->m_nPaintHeight;j++) {
		Map(target,i,j) = Map(source,i,j);
	}
}
}
