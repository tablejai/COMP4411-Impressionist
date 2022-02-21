//
// TraingleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "starbrush.h"
#include "math.h"
#include <iostream>
using namespace std;
extern float frand();

StarBrush::StarBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void StarBrush::BrushBegin(const Point source, const Point target)
{

	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	size = pDoc->getSize();
	alpha = pDoc->getAlpha();
	glDisable(GL_POINT_SMOOTH);
	glPointSize((float)size);
	BrushMove(source, target);
}


void StarBrush::stardraw(const Point target) {
	
	glBegin(GL_POLYGON);
	glVertex2d(target.x, target.y + size);
	glVertex2d(target.x + size * cos(30.0 / 180 * M_PI), target.y - size * sin(30.0 / 180 * M_PI));
	glVertex2d(target.x - size * cos(30.0 / 180 * M_PI), target.y - size * sin(30.0 / 180 * M_PI));
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(target.x, target.y - size);
	glVertex2d(target.x + size * cos(30.0 / 180 * M_PI), target.y + size * sin(30.0 / 180 * M_PI));
	glVertex2d(target.x - size * cos(30.0 / 180 * M_PI), target.y + size * sin(30.0 / 180 * M_PI));
	glEnd();
}

void StarBrush::BrushMove(const Point source, const Point target)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	SetColor(source);
	stardraw(target);
}

void StarBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

