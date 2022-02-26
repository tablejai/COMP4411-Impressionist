//
// WarpBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "Warp.h"
using namespace std;
Warp::Warp(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}
vector<vector<double>> Warp::getGkernel(float sig, int size)
{
    vector<vector<double>> kernel;
	for (int i = 0; i < size; i++) {
		vector<double> kernelrow;
		for (int j = 0; j < size; j++) {
			int xpos = i - size / 2;
			int ypos = j - size / 2;
			float vpos = (1 / (2 * M_PI * sig)) * exp(-(xpos * xpos + ypos * ypos) / (2 * sig));
			kernelrow.push_back(vpos);
		}
		kernel.push_back(kernelrow);
	}
	return kernel;
}
void Warp::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	prev_pos.x = source.x;
	prev_pos.y = source.y;
	kernelSize = pDoc->getSize();
	alpha = pDoc->getAlpha();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	BrushMove(source, target);
}

void Warp::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* pUI = pDoc->m_pUI;
	if (source.x > pDoc->m_nPaintWidth || source.y < 0)
	{
		return;
	}
	Point curr = source;
	 kernelSize = pDoc->getSize();
	 int size = kernelSize;
	 float sig = 100;
	 for (int i = 0; i < size; i++) {
		 vector<double> kernelrow;
		 for (int j = 0; j < size; j++) {
			 int xpos = i - size / 2;
			 int ypos = j - size / 2;
			 float vpos = (1 / (2 * M_PI * sig)) * exp(-(xpos * xpos + ypos * ypos) / (2 * sig));
			 kernelrow.push_back(vpos);
		 }
		 warpkernel.push_back(kernelrow);
	 }
	for (int i = 0; i < kernelSize; i++)
	{
		for (int j = 0; j < kernelSize; j++)
		{
			if (sqrt(pow(i - kernelSize / 2, 2) + pow(j - kernelSize / 2, 2)) > kernelSize / 2)
			{
				warpkernel[i][j] = 0;
			}
		}
	}
	wrapfunc(prev_pos, source, kernelSize);
	prev_pos.x = source.x;
	prev_pos.y = source.y;
}

void Warp::BrushEnd(const Point source, const Point target)
{
}

void Warp::wrapfunc(const Point prev_pos, const Point current_pos, int size)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* pUI = pDoc->m_pUI;
;
	int prevx = prev_pos.x;
	int prevy = prev_pos.y;
	int height = pUI->m_paintView->m_nDrawHeight;
	int width = pUI->m_paintView->m_nDrawWidth;
	int index = prevx - size / 2;
	cout<<"wh" << width << "," << height << endl;
	double direction = atan2((prevy-current_pos.y),( prevx- current_pos.x));
	double dist = sqrt(pow(prevx -current_pos.x, 2) + pow(prevy - current_pos.x, 2))/2.0;
	glPointSize((float)1);
	glBegin(GL_POINTS);

	for (  int i = 0;  index < (prevx + size / 2);  i++)
	{
		if (index<0 || index>width - 1)
			continue;

		int index2 = prevy - size / 2;

		for (int j = 0; index2 < (prevy + size / 2); index2++, j++)
		{
			if (!(index2<0 || index2>height - 1) && !(sqrt(pow(index - prevx, 2) + pow(index2 - prevy, 2)) > size / 2)) {
				double tempx = index - warpkernel[i][j] * (current_pos.x - prevx) / warpkernel[size / 2][size / 2];
				double tempy = index2- warpkernel[i][j] * (current_pos.y - prevy) / warpkernel[size / 2][size / 2];
				GLubyte color[3];
				color[0] = Map(pDoc->m_ucBitmap, 3 * (int)tempx, (int)tempy, pUI->m_paintView->m_nDrawWidth );
				color[1] = Map(pDoc->m_ucBitmap, 3 * (int)tempx+1, (int)tempy, pUI->m_paintView->m_nDrawWidth);
				color[2] = Map(pDoc->m_ucBitmap, 3 * (int)tempx + 2, (int)tempy, pUI->m_paintView->m_nDrawWidth);
				glColor4ub(color[0],color[1],color[2],255*alpha);
				glVertex2d(index, index2);
			}
		}
		index++;
	}
	glEnd();

}

