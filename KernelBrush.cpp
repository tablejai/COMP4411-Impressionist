#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "KernelBrush.h"
#include "math.h"
#include "time.h"
#include <FL/fl_Color_Chooser.h>
#include <iostream>
using namespace std;


#define PYTHAGOREAN(a,b) sqrt(a*a + b*b)

KernelBrush::KernelBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void KernelBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	size = pDoc->getSize();
	alpha = pDoc->getAlpha();
	BrushMove(source, target);
}

void KernelBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int w = pDoc->m_nPaintWidth;
	int h = pDoc->m_nPaintHeight;
	std::cout << "Size: " << size << std::endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int x1 = source.x + i - size / 2;
			int x2 = target.x + i - size / 2;
			int y1 = source.y + j - size / 2;
			int y2 = target.y + j - size / 2;
			if (x1 > (w - 1) || x2 < 0 || y1 >(h - 1) || y2 < 0) {
				continue;
			}
			//std::cout << x2 << " " << y2 << std::endl;
			vector<int> outputVal = Kernel(Point(x1, y1), Point(x2, y2));
			KernelSetColor(Point(x2,y2), outputVal[0], outputVal[1], outputVal[2], 0, true);
		}
	}
	 dlg->m_paintView->RestorePreviousDataRGBA(dlg->m_paintView->rgbaBrush, GL_BACK);
	 //dlg->m_paintView->RestoreContent(GL_BACK);
}

vector<int> KernelBrush::Kernel(const Point source, const Point target) {
	int rSum = 0, bSum = 0, gSum = 0;
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int w = pDoc->m_nPaintWidth;
	int h = pDoc->m_nPaintHeight;
	int kernelSize = kernel.size();
	for (int i = 0; i < kernelSize; i++) {
		for (int j = 0; j < kernelSize; j++) {
			int srcX = source.x + i - kernelSize / 2;
			int srcY = source.y + j - kernelSize / 2;
			if (srcX > (w - 1) || srcX < 0 || srcY >(h - 1) || srcY < 0) {
				continue;
			}
			
			GLubyte srcColor[3];
			memcpy(srcColor, pDoc->GetOriginalPixel(Point(srcX, srcY)), 3);
			
			float kernelVal = kernel[i][j];
			
			rSum += srcColor[0] * kernelVal;
			gSum += srcColor[1] * kernelVal;
			bSum += srcColor[2] * kernelVal;

		}
	}
	vector<int> output = { rSum, gSum, bSum };
	return output;

}

void KernelBrush::KernelSetColor(const Point target, const int r, const int g, const int b, int tarArr, bool has_alpha) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int w = pDoc->m_nPaintWidth;
	int h = pDoc->m_nPaintHeight;

	Fl_Color_Chooser* colorChooser = dlg->m_ColorChooser;

	int r_mult = colorChooser->r();
	int g_mult = colorChooser->g();
	int b_mult = colorChooser->b();
	if (tarArr == 0) {
		dlg->m_paintView->rgbaBrush[(target.x + target.y * w) * 4] = min(max(r, 0), 255) * r_mult;
		dlg->m_paintView->rgbaBrush[(target.x + target.y * w) * 4 + 1] = min(max(g, 0), 255) * g_mult;
		dlg->m_paintView->rgbaBrush[(target.x + target.y * w) * 4 + 2] = min(max(b, 0), 255) * b_mult;
		dlg->m_paintView->rgbaBrush[(target.x + target.y * w) * 4 + 3] = alpha * 255;
	}
	else if(tarArr == 1) {
		pDoc->m_edgeView[(target.x + target.y * w) * 3] = min(max(r, 0), 255) * r_mult;
		pDoc->m_edgeView[(target.x + target.y * w) * 3 + 1] = min(max(g, 0), 255) * g_mult;
		pDoc->m_edgeView[(target.x + target.y * w) * 3 + 2] = min(max(b, 0), 255) * b_mult;
	}
	else if (tarArr == 2) {
		denoisedImg[(target.x + target.y * w) * 3] = min(max(r, 0), 255);
		denoisedImg[(target.x + target.y * w) * 3 + 1] = min(max(g, 0), 255);
		denoisedImg[(target.x + target.y * w) * 3 + 2] = min(max(b, 0), 255);
	}
}

void KernelBrush::SobelOperator(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int w = pDoc->m_nPaintWidth;
	int h = pDoc->m_nPaintHeight;

	if (pDoc->m_edgeView != nullptr) {
		delete [] pDoc->m_edgeView;
	}
	pDoc->m_edgeView = new unsigned char[w * h * 4];
	Denoise(source, target);

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	vector<vector<float>> sobel_x = { {-1, 0, 1}, {-2,0,2}, {-1,0,1} };
	vector<vector<float>> sobel_y = { {1, 2, 1}, {0,0,0}, {-1, -2, -1} };

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int x1 = source.x + i - size / 2;
			int x2 = target.x + i - size / 2;
			int y1 = source.y + j - size / 2;
			int y2 = target.y + j - size / 2;
			if (x1 > (w - 1) || x2 < 0 || y1 >(h - 1) || y2 < 0) {
				continue;
			}
			//std::cout << x2 << " " << y2 << std::endl;
			kernel = sobel_x;
			vector<int> outputValX = Kernel(Point(x1, y1), Point(x2, y2));
			kernel = sobel_y;
			vector<int> outputValY = Kernel(Point(x1, y1), Point(x2, y2));

			vector<int> setVal = { (int)PYTHAGOREAN(outputValX[0], outputValY[0]), (int)PYTHAGOREAN(outputValX[1] , outputValY[1]), (int)PYTHAGOREAN(outputValX[2], outputValY[2]) };

			setVal[0] = min(max(setVal[0], 0), 255);
			setVal[1] = min(max(setVal[1], 0), 255);
			setVal[2] = min(max(setVal[2], 0), 255);

			KernelSetColor(Point(x2, y2), setVal[0], setVal[1], setVal[2], 1, false);
		}
	}
	dlg->m_origView->showEdge = true;
	dlg->m_origView->refresh();
}

void KernelBrush::Denoise(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int w = pDoc->m_nPaintWidth;
	int h = pDoc->m_nPaintHeight;
	if (denoisedImg == nullptr) {
		delete[] denoisedImg;
	}

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	vector<vector<float>> meanFilter = { {1/9, 1/9, 1/9}, {1/9,1/9,1/9}, {1/9,1/9,1/9} };

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int x1 = source.x + i - size / 2;
			int x2 = target.x + i - size / 2;
			int y1 = source.y + j - size / 2;
			int y2 = target.y + j - size / 2;
			if (x1 > (w - 1) || x2 < 0 || y1 >(h - 1) || y2 < 0) {
				continue;
			}
			kernel = meanFilter;
			vector<int> outputVal = Kernel(Point(x1, y1), Point(x2, y2));

			KernelSetColor(Point(x2, y2), outputVal[0], outputVal[1], outputVal[2], 0, false);
		}
	}
}

void KernelBrush::BrushEnd(const Point source, const Point target)
{

}

