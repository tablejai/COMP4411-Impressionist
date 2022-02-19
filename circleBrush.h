
#ifndef SINGLE_CIRCLE
#define SINGLE_CIRCLE

#include "ImpBrush.h"
class CircleBrush : public ImpBrush
{
	GLfloat radius = 4;
public:
	CircleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void circle(int, int,int);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif
