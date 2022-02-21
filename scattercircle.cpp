//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scattercircle.h"
#include "math.h"
#include "time.h"
#include <iostream>
#include <random>
using namespace std;
extern float frand();

#define PI 3.14159265358979323846


ScatterCircleBrush::ScatterCircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatterCircleBrush::BrushBegin(const Point source, const Point target)
{

	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	radius = pDoc->getSize();
	alpha = pDoc->getAlpha()*pDoc->getAlpha()* pDoc->getAlpha();

	BrushMove(source, target);
}

void ScatterCircleBrush::BrushMove(const Point source, const Point target)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(1);
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	GLfloat x_pos, y_pos;
	GLfloat angle;
	int realRadius;
	auto seed = mt19937{ random_device()() };
	mt19937 mt(seed);
	int randNum;

	if (bmode == NORMALMODE) {
		realRadius = radius;
		randNum = abs((int)mt()) % (2) + 2 / radius;

	}
	else {
		realRadius = radius + ((int)mt()) % 4;
		randNum = 2;
	}
	for (int i = 0;i < randNum;i++) {
		GLfloat xchange = ((int)(mt()*i+ mt()))%(realRadius)-realRadius / 2;
		GLfloat ychange = ((int)(mt()*i+ mt()))%(realRadius)-realRadius / 2;
		Point temp = {(int)(source.x+xchange),(int)(source.y+ychange )};
		SetColor(temp);
		x_pos = target.x + xchange;
		y_pos = target.y + ychange;
		circle(x_pos, y_pos, realRadius);
	}
}

void ScatterCircleBrush::BrushEnd(const Point source, const Point target)
{

}

void ScatterCircleBrush::circle(int target_x, int target_y,int radius) {
	glBegin(GL_POLYGON);

	for (int i = 0; i < 360; ++i) {
		float theta = i * PI / 180;
		glVertex2f(target_x + radius * cos(theta), target_y + radius * sin(theta));
	}

	glEnd();
}

#undef PI