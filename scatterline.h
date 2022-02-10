#ifndef SCATTER_LINE
#define SCATTER_LINE

#include "ImpBrush.h"
class ScatterLineBrush : public ImpBrush
{
	double width = 10;
	double height = 1;
	GLfloat alpha = 0;
	GLfloat angle = 0;
public:
	ScatterLineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	double meanfilter(const Point source);
	double getGradientAngle(const Point source);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif