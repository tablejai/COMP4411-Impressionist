#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "BlurringBrush.h"
#include "math.h"
#include "time.h"
#include <iostream>

BlurringBrush::BlurringBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void BlurringBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	size = pDoc->getSize();
	alpha = pDoc->getAlpha() * pDoc->getAlpha() * pDoc->getAlpha();
	BrushMove(source, target);
}

void BlurringBrush::BrushMove(const Point source, const Point target)
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
			int x1 = target.x + i;
			int x2 = target.x - i;
			int y1 = target.y + j;
			int y2 = target.y - j;
			if (x1 > (w - 1) || x2 < 0 || y1 >(h - 1) || y2 < 0) {
				continue;
			}
			BlurringKernel(source, Point(x1, y1));
		}
	}
}

void BlurringBrush::BlurringKernel(const Point source, const Point target) {
	int rSum = 0, bSum = 0, gSum = 0;
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int w = pDoc->m_nPaintWidth;
	int h = pDoc->m_nPaintHeight;

	const int kernelSize = 3;
	for (int i = 0; i < kernelSize; i++) {
		for (int j = 0; j < kernelSize; j++) {
			int srcX = source.x + i - kernelSize / 2;
			int srcY = source.y + j - kernelSize / 2;
			if (srcX > (w - 1) || srcX < 0 || srcY >(h - 1) || srcY < 0) {
				continue;
			}
			GLubyte color[3];
			memcpy(color, pDoc->GetOriginalPixel(source), 3);

			rSum += color[0];
			gSum += color[1];
			bSum += color[2];
		}
	}
	rSum /= kernelSize * kernelSize;
	bSum /= kernelSize * kernelSize;
	gSum /= kernelSize * kernelSize;

	KernelSetColor(target, rSum, gSum, bSum);

	dlg->m_paintView->RestoreContent(GL_BACK);
}

void BlurringBrush::KernelSetColor(const Point target, const int r, const int g, const int b) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int w = pDoc->m_nPaintWidth;
	int h = pDoc->m_nPaintHeight;

	pDoc->m_ucPainting[(target.x + target.y * w) * 3] = r;
	pDoc->m_ucPainting[(target.x + target.y * w) * 3 + 1] = g;
	pDoc->m_ucPainting[(target.x + target.y * w) * 3 + 2] = b;
}


void BlurringBrush::BrushEnd(const Point source, const Point target)
{
}

 