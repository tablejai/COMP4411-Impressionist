//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "AlphaMapBrush.h"
#include <iostream>
using namespace std;

extern float frand();

AlphaBrush::AlphaBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
	alphaMap = nullptr;
	mapw = 0;
	maph = 0;
}

void AlphaBrush::updateAlphaImageBrush(unsigned char* data,int w, int h) {
	if (alphaMap)
		delete [] alphaMap;
	if (w > 0 && h > 0) {
		alphaMap = new unsigned char[w * h * 3];
		GLubyte color[3];
		mapw = w;
		maph = h;
		for (int i = 0;i < h;i++) {
			for (int j = 0;j < w;j++) {
				color[0] = Map(data, 3 * j, i, w);
				color[1] = Map(data, 3 * j + 1, i, w);
				color[2] = Map(data, 3 * j + 2, i, w);
				int intensity = RGB_TO_INTENSITY(color[0], color[1], color[2]);
				Map(alphaMap, 3 * j, i, w) = intensity;
				Map(alphaMap, 3 * j + 1, i, w) = intensity;
				Map(alphaMap, 3 * j + 2, i, w) = intensity;
			}
		}
	}
}


void AlphaBrush::BrushBegin(const Point source, const Point target)
{

	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	size = pDoc->getSize();
	alpha = pDoc->getAlpha();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initReverseData();
	glPointSize((float)size);
	BrushMove(source, target);
}
void AlphaBrush::updateReverData() {

}

void AlphaBrush::BrushMove(const Point source, const Point target)
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawBuffer(GL_BACK);
	glPointSize((float)size);
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	glBegin(GL_POINTS);
	
	for (int j = 0;j < maph;j++) {
		for (int i = 0;i < mapw;i++) {
			Point drawpt = { target.x + i - mapw / 2 , target.y + j -maph/2};
			if (0<=drawpt.x && drawpt.x< pDoc->m_nPaintWidth && 0<=drawpt.y &&drawpt.y<pDoc->m_nPaintHeight) {
				unsigned char avg = Map(alphaMap, 3 * i, j, mapw); //since r g b has same value take one is ok :D
				alpha = avg/255.0;
				SetColor(source);
				glVertex2i(drawpt.x, drawpt.y);
			}
		}
	}
	glEnd();
}

void AlphaBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

