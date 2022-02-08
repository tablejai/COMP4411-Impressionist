//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circle.h"
#include "math.h"
extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	radius = pDoc->getSize();
	alpha = pDoc->getAlpha()*pDoc->getAlpha()*pDoc->getAlpha();
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(radius);
	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	GLfloat x_pos , y_pos;
	glBegin(GL_POINTS);
	GLfloat angle;
	GLubyte color[3];
	memcpy(color, pDoc->GetOriginalPixel(source), 3);
	glColor4f(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, (GLfloat)alpha);
	glVertex3f(target.x, target.y, 0.0f);
	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	glDisable(GL_POINT_SMOOTH);
	// do nothing so far
}

