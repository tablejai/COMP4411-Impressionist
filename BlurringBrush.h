#ifndef BLURRING_BRUSH
#define  BLURRING_BRUSH

#include "ImpBrush.h"
#include "KernelBrush.h"
class BlurringBrush : public KernelBrush
{
	int size = 10;
public:
	BlurringBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	char* BrushName(void);
};

#endif