
#ifndef TRAINBRUSH_H
#define TRAINBRUSH_H

#include "ImpBrush.h"

class TraingleBrush : public ImpBrush
{
	GLfloat size = 0;
public:
	TraingleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void traingledraw(const Point target);
	char* BrushName(void);
};

#endif