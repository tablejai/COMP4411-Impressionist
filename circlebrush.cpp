//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circleBrush.h"
#include "math.h"
#include <iostream>
using namespace std;
#define PI 3.14159265358979323846

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	radius = pDoc->getSize();
	alpha = pDoc->getAlpha()*pDoc->getAlpha()*pDoc->getAlpha();
	//BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	glPointSize(1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	GLfloat x_pos , y_pos;
	GLfloat angle;
	SetColor(source);
	circle(target.x, target.y);

}

void CircleBrush::circle(int target_x, int target_y) {
	glBegin(GL_POLYGON);

	for (int i = 0; i < 360; ++i) {
		float theta = i * PI / 180;
		glVertex2f(target_x + radius * cos(theta), target_y + radius * sin(theta));
	}

	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	//glDisable(GL_POINT_SMOOTH);
	// do nothing so far
}

#undef PI