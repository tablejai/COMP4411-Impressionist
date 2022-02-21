#ifndef SHARPENING_BRUSH
#define  SHARPENING_BRUSH

#include "ImpBrush.h"
#include "KernelBrush.h"
class SharpeningBrush : public KernelBrush
{
	int size = 10;
public:
	SharpeningBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	char* BrushName(void);
};

#endif