//
// CurveBrush.h
//
// The header file for Point Brush. 
//

#ifndef CRUVEBRUSH_H
#define CRUVEBRUSH_H

#include "ImpBrush.h"

class CurveBrush : public ImpBrush
{
	int size = 0;
	int maxStrokeLength;
	int 	minStrokeLength;

public:
	CurveBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	double vectorDistance(const vector<int>& v1, const vector<int>& v2);
	BrushStroke PaintStroke(int targetx, int targety, unsigned char* canvas, int w, int h, int radius, unsigned char* target, float kernel);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void updateReverData();
	char* BrushName(void);
};

#endif