#ifndef KERNEL_BRUSH
#define  KERNEL_BRUSH

#include "ImpBrush.h"
#include <vector>

class KernelBrush : public ImpBrush
{
	int size = 10;

public:
	std::vector<std::vector<float>> kernel;
	KernelBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void Kernel(const Point source, const Point target);
	void KernelSetColor(const Point target, const int r, const int g, const int b);
	char* BrushName(void);
};

#endif