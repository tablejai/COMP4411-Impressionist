#ifndef KERNEL_BRUSH
#define  KERNEL_BRUSH

#include "ImpBrush.h"
#include <vector>
using namespace std;

class KernelBrush : public ImpBrush
{

public:
	int size = 10;
	unsigned char* denoisedImg = nullptr;
	std::vector<std::vector<float>> kernel;
	KernelBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	vector<int> Kernel(const Point source, const Point target);
	void KernelSetColor(const Point target, const int r, const int g, const int b, int ,bool);
	void SobelOperator(const Point, const Point);
	void Denoise(const Point, const Point);
	char* BrushName(void);
};

#endif