
#ifndef SINGLE_CIRCLE
#define SINGLE_CIRCLE



#include "ImpBrush.h"
class CircleBrush : public ImpBrush
{
	GLfloat radius = 4;
	GLfloat alpha = 0;
public:
	CircleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif
