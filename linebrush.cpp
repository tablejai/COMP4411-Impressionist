//
// linebrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "linebrush.h"
#include <iostream>
using namespace std;

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
    this->width = pDoc->getSize();
	this->height = pDoc->getWidth();
	this->alpha = pDoc->getAlpha();
	this->angle = pDoc->getAngle();
	BrushMove(source, target);

	
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	if (source.y < 0 || source.x > m_nWindowWidth) return;

	glPushMatrix();
	glTranslatef(target.x, target.y, 0.0f);
	glRotatef(angle, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	GLubyte color[3];
	memcpy(color, pDoc->GetOriginalPixel(source), 3);
	glColor4f(color[0]/255.0f, color[1]/255.0f, color[2]/255.0f, (float)alpha);
    cout << (int)color[0] << "," << (int)color[1]<<","<< (int)color[2]<<","<<alpha<< endl;
	cout << angle << endl;
	glVertex2d(- width/2, - height/2);
	glVertex2d( - width / 2,  height / 2);
	glVertex2d( + width / 2,  height / 2);
	glVertex2d( + width / 2,  - height / 2);

	glEnd();
	glPopMatrix();

}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
