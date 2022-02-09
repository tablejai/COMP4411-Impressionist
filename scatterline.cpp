//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatterline.h"
#include "time.h"
extern float frand();

ScatterLineBrush::ScatterLineBrush(ImpressionistDoc* pDoc, char* name) :ImpBrush(pDoc, name)
{
}

void ScatterLineBrush::BrushBegin(const Point source, const Point target)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	if (source.y < 0 || source.x > m_nWindowWidth) return;

	srand(time(NULL));
	int randNum = rand() % (5)+3;
	glPushMatrix();
	glTranslatef(target.x, target.y, 0.0f);
	glRotatef(angle, 0.0, 0.0, 1.0);
	for (int i = 0;i < randNum;i++) {
		srand(target.x*target.y%400);
		double xchange = rand()%(int)width-width/2;
		double ychange = rand()%(int)width-width / 2;
		glBegin(GL_POLYGON);
		GLubyte color[3];
		memcpy(color, pDoc->GetOriginalPixel(source), 3);
		glColor4f(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, (float)alpha);
		glVertex2d( xchange - width / 2,  ychange - height / 2);
		glVertex2d( xchange - width / 2,  ychange +height / 2);
		glVertex2d( xchange + width / 2,  ychange+ height / 2);
		glVertex2d( xchange + width / 2,  ychange - height / 2);
		glEnd();
	}
	glPopMatrix();

}

void ScatterLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

