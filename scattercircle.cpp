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
using namespace std;
extern float frand();


ScatterCircleBrush::ScatterCircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatterCircleBrush::BrushBegin(const Point source, const Point target)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	radius = pDoc->getSize();
	alpha = pDoc->getAlpha()*pDoc->getAlpha()* pDoc->getAlpha();
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(radius);
	BrushMove(source, target);
}

void ScatterCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	GLfloat x_pos, y_pos;
	glBegin(GL_POINTS);
	GLfloat angle;
	GLubyte color[3];
	memcpy(color, pDoc->GetOriginalPixel(source), 3);
	glColor4f(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, (float)alpha);
	int randNum = rand() % (30) + 30/radius;
	for (int i = 0;i < randNum;i++) {
	srand(target.x * target.y*time(NULL) % 400);
	GLfloat xchange = (rand()*i+rand())%(radius)- radius/ 2;
	GLfloat ychange = (rand()*i+rand())%(radius)- radius/ 2;
	x_pos = target.x + xchange;
	y_pos = target.y + ychange;
	glVertex3f(x_pos, y_pos, 0.0f);
	glEnd();
	}
}

void ScatterCircleBrush::BrushEnd(const Point source, const Point target)
{
	cout << "triggerend scattercircle" << endl;
	glDisable(GL_POINT_SMOOTH);

}

