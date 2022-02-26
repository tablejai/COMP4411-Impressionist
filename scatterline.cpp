//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatterline.h"
#include "math.h"
#include "time.h"
#include <random>
using namespace std;
extern float frand();

ScatterLineBrush::ScatterLineBrush(ImpressionistDoc* pDoc, char* name) :ImpBrush(pDoc, name)
{
}

double ScatterLineBrush::meanfilter(const Point source) {
	ImpressionistDoc* pDoc = GetDocument();
	double meanintensity = 0;
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
double ScatterLineBrush::getGradientAngle(const Point source) {
	ImpressionistDoc* pDoc = GetDocument();
	Point tempR(source.x + 1, source.y);
	Point tempD(source.x, source.y - 1);
	double intensity = meanfilter(source);
	double intensity1 = meanfilter(tempR);
	double intensity2 = meanfilter(tempD);
	double gradientx = (intensity1 - intensity) / 2;
	double gradienty = (intensity - intensity2) / 2;
	return atan(gradienty / gradientx);
}

void ScatterLineBrush::BrushBegin(const Point source, const Point target)
{
	
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	width = pDoc->getSize();
	height = pDoc->getWidth();
	alpha = pDoc->getAlpha();
	angle = pDoc->getAngle();
	glPointSize((float)1.0f);
	BrushMove(source, target);
}


void ScatterLineBrush::BrushMove(const Point source, const Point target)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	auto seed = mt19937{ random_device()() };
	mt19937 mt(seed);
	double realwidth, realheight;
	int angledelta;
	if (bmode == NORMALMODE) {
		realwidth = width;
		realheight = height;
		angledelta = 0;
	}
	else
	{
		int temp = (width > 1) ? (width / 2) : width;
		realwidth = width + ((int)mt()) % (temp);
		temp = (height > 1) ? (height / 2) : height;
		realheight = height + ((int)mt()) % (temp);
		angledelta = ((int)mt()) % (40);
	}
	srand(time(NULL));
	int randNum = rand() % (5)+3;
	glPushMatrix();
	glTranslatef(target.x, target.y, 0.0f);
	if (pDoc->c_pStrokes == STROKE_BRUSH_DIRECTION) {
		angle = -((atan((double)mouseVec.y / mouseVec.x)) / M_PI * 180)+ angledelta;
		glRotatef(angle, 0.0, 0.0, 1.0);


	}
	else if (pDoc->c_pStrokes == STROKE_GRADIENT) {

		angle = getGradientAngle(source) / M_PI * 180 + 90;
		glRotatef(angle + angledelta, 0.0, 0.0, 1.0);

	}
	else {
		angle = pDoc->getAngle();
		glRotatef(angle + angledelta, 0.0, 0.0, 1.0);

	}

	for (int i = 0;i < randNum;i++) {
		srand(target.x*target.y%400);
		double xchange = rand()%(int)realwidth - realwidth /2;
		double ychange = rand()%(int)realwidth - realwidth / 2;
		glBegin(GL_POLYGON);
		SetColor(source);

		int w = dlg->m_paintView->m_nDrawHeight;
		int h = dlg->m_paintView->m_nDrawWidth;
		if (clip) {
			float startWidth = width / 2, endWidth = width / 2;
			for (int i = 0; i < (width / 2); i++) {
				Point startPt = Point(target.x + xchange + i * cos(angle), target.y +ychange + i * sin(angle));

				if (startPt.x < 0 || startPt.x > w - 1 || startPt.y < 0 || startPt.y > h - 1 || pDoc->isEdge(startPt)) {
					startWidth = i;
					break;
				}
			}

			for (int i = 0; i < (width / 2); i++) {
				Point endPt = Point(target.x +xchange + i * cos(angle + M_PI), target.y + ychange+i * sin(angle + M_PI));

				if (endPt.x < 0 || endPt.x > w - 1 || endPt.y < 0 || endPt.y > h - 1 || pDoc->isEdge(endPt)) {
					endWidth = i;
					break;
				}
			}
			glVertex2d(xchange -startWidth / 2,ychange -realheight / 2);
			glVertex2d(xchange -startWidth / 2, ychange +realheight / 2);
			glVertex2d(xchange +endWidth / 2, ychange +realheight / 2);
			glVertex2d(xchange +endWidth / 2, ychange -realheight / 2);

		}
		else {
			glVertex2d(xchange - realwidth / 2, ychange - realheight / 2);
			glVertex2d(xchange - realwidth / 2, ychange + realheight / 2);
			glVertex2d(xchange + realwidth / 2, ychange + realheight / 2);
			glVertex2d(xchange + realwidth / 2, ychange - realheight / 2);
		}
	}
	glEnd();
	glPopMatrix();

}

void ScatterLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

