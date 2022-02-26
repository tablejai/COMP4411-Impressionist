#ifndef KERNEL_BRUSH
#define  KERNEL_BRUSH

#include "ImpBrush.h"
#include <vector>
using namespace std;

class KernelBrush : public ImpBrush
{

public:
	int size = 10;
	std::vector<std::vector<float>> kernel;
	KernelBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	vector<int> Kernel(const Point source, const Point target);
	void KernelSetColor(const Point target, const int r, const int g, const int b, bool ,bool);
	void SobelOperator(const Point, const Point);
	char* BrushName(void);
};

#endif