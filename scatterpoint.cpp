//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatterpoint.h"
#include <cmath>
#include "math.h"
#include <iostream>
#include <random>
using namespace std;
#include "time.h"
extern float frand();

ScatterPointBrush::ScatterPointBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatterPointBrush::BrushBegin(const Point source, const Point target)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	glDisable(GL_POINT_SMOOTH);
	range = pDoc->getSize();
	alpha = pDoc->getAlpha();
	cout << "trigger begin" << endl;
	cout << alpha << endl;
	glPointSize((float)1.0);
	BrushMove(source, target);
}

void ScatterPointBrush::BrushMove(const Point source, const Point target)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	auto seed = mt19937{ random_device()() };
	mt19937 mt(seed);
	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	if (bmode == NORMALMODE) {
		glPointSize((float)1.0);
	}
	else {
		glPointSize((float)(abs((int)mt()) % 5));
	}
	int x_pos, y_pos;
	GLfloat angle;

	glBegin(GL_POINTS);
	for (int i = -range;i <= range;i++) {
		for (int j = -range;j <= range;j++) {
			Point pt = {source.x+i,source.y+j};
			SetColor(pt);
			int display = (abs((int)mt())) % 10;
			if (display<1) {
				glVertex3f(target.x + i, target.y + j, 0.0f);
			}
		}
	}
	glEnd();
}

void ScatterPointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

