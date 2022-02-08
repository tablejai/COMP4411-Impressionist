#ifndef  SCATTER_POINT
#define  SCATTER_POINT

#include "ImpBrush.h"
class ScatterPointBrush : public ImpBrush
{
	int range = 10;
	GLfloat alpha = 0;
public:
	ScatterPointBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif