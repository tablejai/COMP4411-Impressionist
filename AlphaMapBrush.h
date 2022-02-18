//
// PointBrush.h
//
// The header file for Point Brush. 
//

#ifndef ALPHABRUSH_H
#define ALPHABRUSH_H

#include "ImpBrush.h"

class AlphaBrush : public ImpBrush
{
	int size = 0;
	unsigned char* alphaMap;
	int mapw;
	int maph;
public:
	AlphaBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void updateAlphaImageBrush(unsigned char* , int , int );
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void updateReverData();
};

#endif