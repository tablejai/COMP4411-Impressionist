#ifndef SHARPENING_BRUSH
#define  SHARPENING_BRUSH

#include "ImpBrush.h"
class SharpeningBrush : public ImpBrush
{
	int size = 10;
public:
	SharpeningBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void SharpeningKernel(const Point source, const Point target);
	void KernelSetColor(const Point target, const int r, const int g, const int b);
	char* BrushName(void);
};

#endif