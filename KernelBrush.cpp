#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "KernelBrush.h"
#include "math.h"
#include "time.h"
#include <FL/fl_Color_Chooser.h>
#include <iostream>

#define MAX(a,b) a>b? b : a
#define MIN(a,b) a<b? b : a

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
			Kernel(Point(x1, y1), Point(x2, y2));
		}
	}
	 dlg->m_paintView->RestorePreviousDataRGBA(dlg->m_paintView->rgbaBrush, GL_BACK);
	 //dlg->m_paintView->RestoreContent(GL_BACK);
}

void KernelBrush::Kernel(const Point source, const Point target) {
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

	KernelSetColor(target, rSum, gSum, bSum); 

}

void KernelBrush::KernelSetColor(const Point target, const int r, const int g, const int b) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int w = pDoc->m_nPaintWidth;
	int h = pDoc->m_nPaintHeight;

	Fl_Color_Chooser* colorChooser = dlg->m_ColorChooser;

	int r_mult = colorChooser->r();
	int g_mult = colorChooser->g();
	int b_mult = colorChooser->b();
	dlg->m_paintView->rgbaBrush[(target.x + target.y * w) * 4] = min(max(r,0), 255) * r_mult;
	dlg->m_paintView->rgbaBrush[(target.x + target.y * w) * 4 + 1] = min(max(g,0), 255) * g_mult;
	dlg->m_paintView->rgbaBrush[(target.x + target.y * w) * 4 + 2] = min(max(b,0), 255) * b_mult;
	dlg->m_paintView->rgbaBrush[(target.x + target.y * w) * 4 + 3] = alpha * 255;
}



void KernelBrush::BrushEnd(const Point source, const Point target)
{

}

