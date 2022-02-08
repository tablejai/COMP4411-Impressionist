#ifndef SCATTER_CIRCLE
#define  SCATTER_CIRCLE

#include "ImpBrush.h"
class ScatterCircleBrush : public ImpBrush
{
	int radius = 10;
	GLfloat alpha = 0;
public:
	ScatterCircleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif