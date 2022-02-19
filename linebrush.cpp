//
// linebrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "linebrush.h"
#include <math.h>
#include "PaintView.h"
#include <iostream>
#include <random>
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
double LineBrush::meanfilter(const Point source) {
	ImpressionistDoc* pDoc = GetDocument();
	double meanintensity =0;
	GLubyte color[3];
	// did not handle 5 edge
	for (int i = -1;i < 2;i++) {
		for (int j = -1;j < 2;j++) {
			Point temp;
			temp.x = source.x + i;
			temp.y = source.y + j;
			memcpy(color, pDoc->GetOriginalPixel(temp), 3);
			meanintensity += RGB_TO_INTENSITY(color[0], color[1], color[2]);
		}
	}
	return meanintensity / 9;

}
double LineBrush::getGradientAngle(const Point source) {
	ImpressionistDoc* pDoc = GetDocument();
	Point tempR(source.x+1,source.y);
	Point tempD(source.x, source.y-1);
	double intensity = meanfilter(source);
	double intensity1 = meanfilter(tempR);
	double intensity2 = meanfilter(tempD);
	double gradientx = (intensity1 - intensity) / 2;
	double gradienty = (intensity - intensity2) / 2;
	return atan( gradienty/ gradientx);
}
void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	auto seed = mt19937{ random_device()() };
	mt19937 mt(seed);
	double realwidth = 0;
	double realheight = 0;
	int angledelta = 0;
	if (bmode == NORMALMODE) {
		realwidth = width;
		realheight = height;
		angledelta = 0;
	}
	else
	{
		int temp = (width > 1) ? (width / 2) : width;
		realwidth = width + (abs((int)mt())) % (temp);
		temp = (height > 1) ? (height / 2) : height;
		realheight = height + (abs((int)mt())) % (temp);
		angledelta = ((int)mt()) % (40);
	}

	glPushMatrix();
	glTranslatef(target.x, target.y, 0.0f);
	if (pDoc->c_pStrokes == STROKE_BRUSH_DIRECTION) {
		angle = (-(int)(atan((double)mouseVec.y / mouseVec.x)/M_PI*180) + 360+ angledelta)%360;
		angle = (315<=angle&&angle<360 || 0 <= angle && angle < 45) ? 0 :
				(45 < angle && angle < 135) ? 90:
			    (135 < angle && angle < 225) ? 180:
				(225 < angle && angle < 315) ? 270:0;
		;
	}
	else if (pDoc->c_pStrokes == STROKE_GRADIENT) {

		angle = getGradientAngle(source)/M_PI*180+90+angledelta;
	}
	else {
		angle = angledelta;
	}
	glRotatef(angle, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	SetColor(source);

	//cout << angle << endl;
	//cout << source.x << "|" << source.y << endl;
	glVertex2d(-realwidth /2, -realheight /2);
	glVertex2d( -realwidth / 2, realheight / 2);
	glVertex2d( +realwidth / 2, realheight / 2);
	glVertex2d( +realwidth / 2,  -realheight / 2);

	glEnd();
	glPopMatrix();

}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
