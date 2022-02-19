#ifndef BLURRING_BRUSH
#define  BLURRING_BRUSH

#include "ImpBrush.h"
class BlurringBrush : public ImpBrush
{
	int size = 10;
public:
	BlurringBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void BlurringKernel(const Point source, const Point target);
	void KernelSetColor(const Point target, const int r, const int g, const int b);
	char* BrushName(void);
};

#endif