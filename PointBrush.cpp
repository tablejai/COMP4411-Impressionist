//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "pointbrush.h"
#include <iostream>
#include <random>
using namespace std;

extern float frand();

PointBrush::PointBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void PointBrush::BrushBegin( const Point source, const Point target )
{
	
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	 size = pDoc->getSize();
	alpha = pDoc->getAlpha();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glDisable(GL_POINT_SMOOTH);
	initReverseData();
	glPointSize( (float)size );
	BrushMove(source, target);
}
void PointBrush::updateReverData() {
	/*for (int i = -size;i < size;i++) {

	}*/
}

void PointBrush::BrushMove( const Point source, const Point target )
{    
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawBuffer(GL_BACK);
	auto seed = mt19937{ random_device()() };
	mt19937 mt(seed);
	if(bmode==NORMALMODE)
	glPointSize((float)size);
	else
	glPointSize((float)size+((int)mt())%4);
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}
	glBegin( GL_POINTS );
	SetColor(source);
	glVertex2d( target.x, target.y );
	glEnd();
}

void PointBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

