#pragma once
#pragma once
#include "ImpBrush.h"
#include <vector>
#include <FL/GL.h>
class Warp : public ImpBrush
{
public:
	~Warp();
	int kernelSize;
	Warp(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	Point prev_pos;
	vector<vector<double>> warpkernel;
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
	void wrapfunc(const Point origin, const Point end, int size);
};