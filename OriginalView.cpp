//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "originalview.h"
#include <iostream>
using namespace std;
#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif

OriginalView::OriginalView(int			x, 
						   int			y, 
						   int			w, 
						   int			h, 
						   const char*	l)
							: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}
void OriginalView::blendImage() {

	float alpha = 1;
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0;i < m_pDoc->m_nWMap1;i++) {
		for (int j = 0;j < m_pDoc->m_nHMap1;j++) {
			GLubyte color[3];
			for (int k = 0;k < 3;k++) {
				color[k] = Map(m_pDoc->m_uctempBitmap1, i*3+k, j, m_pDoc->m_nWMap1);
			}
			glBegin(GL_POINTS);
			glColor4f(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, alpha);
			glVertex2d(i, j+ m_pDoc->m_nHeight - m_pDoc->m_nHMap1);
			glEnd();
		}
	alpha -= 0.5/m_pDoc->m_nWMap1;
	}
	alpha = 0;
	for (int i = 0;i < m_pDoc->m_nWMap2;i++) {
		for (int j = 0;j < m_pDoc->m_nHMap2;j++) {
			GLubyte color[3];
			for (int k = 0;k < 3;k++) {
				color[k] = Map(m_pDoc->m_uctempBitmap2, i + k, j, m_pDoc->m_nWMap2);
			}
		glBegin(GL_POINTS);
		glColor4f(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, alpha);
			glVertex2d(i+ m_pDoc->m_nWMap1/2, j+ m_pDoc->m_nHeight - m_pDoc->m_nHMap2);
		glEnd();
	}
	alpha += 0.5 / m_pDoc->m_nWMap2;
	}
}

void OriginalView::draw()
{
	if(!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer( GL_FRONT );
		ortho();

	}

	glClear( GL_COLOR_BUFFER_BIT );

	if ( m_pDoc->m_ucBitmap ) 
	{
		// note that both OpenGL pixel storage and the Windows BMP format
		// store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
		// around with startrow.

		m_nWindowWidth=w();
		m_nWindowHeight=h();

		int drawWidth, drawHeight;
		GLvoid* bitstart;

		// we are not using a scrollable window, so ignore it
		Point scrollpos;	// = GetScrollPosition();
		scrollpos.x=scrollpos.y=0;

		drawWidth	= min( m_nWindowWidth, m_pDoc->m_nWidth );
		drawHeight	= min( m_nWindowHeight, m_pDoc->m_nHeight );

		int	startrow	= m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
		if ( startrow < 0 ) 
			startrow = 0;


		bitstart = m_pDoc->m_ucBitmap + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);

		// just copy image to GLwindow conceptually
		
		if (state==BLEND_VIEW) {
			blendImage();
		}
		else {
		glRasterPos2i( 0, m_nWindowHeight - drawHeight );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth );
		glDrawBuffer( GL_BACK );
		glDrawPixels(drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart);
		}
		m_pDoc->m_pCursor->drawcursor();
	
	}
			
	glFlush();

}

void OriginalView::refresh()
{
	redraw();
}

int OriginalView::getHeight() {

	return m_nWindowHeight;

}
int OriginalView::getWidth() {

	return m_nWindowWidth;

}
void OriginalView::resizeWindow(int	width, 
								int	height)
{
	resize(x(), y(), width, height);
}
void OriginalView::triggerupdate() {

	redraw();
}


