#ifndef IMPBRUSH_H
#define IMPBRUSH_H

//
// ImpBrush.h
//
// The header file of virtual brush. All the other brushes inherit from it.
//

#include <stdlib.h>
#include <FL/fl_Color_Chooser.h>

// Each brush type has an associated constant.
enum
{
	BRUSH_POINTS = 0,
	BRUSH_LINES,
	BRUSH_CIRCLES,
	BRUSH_SCATTERED_POINTS,
	BRUSH_SCATTERED_LINES,
	BRUSH_SCATTERED_CIRCLES,
	BRUSH_TRAINGLE,
	BRUSH_STAR,
	BRUSH_BLURRRING,
	BRUSH_SHARPENING,
	NUM_BRUSH_TYPE // Make sure this stays at the end!
};

typedef enum
{
	STROKE_SLIDER_OR_RIGHT_MOUSE = 0,
	STROKE_GRADIENT,
	STROKE_BRUSH_DIRECTION,
	NUM_STROKE_TYPE
} StrokeDir;

class ImpressionistDoc; // Pre-declaring class

class Point
{
public:
	Point(){};
	Point(int xx, int yy)
	{
		x = xx;
		y = yy;
	};

	int x, y;
};
class Vec2
{
public:
	Vec2() {};
	Vec2(float xx, float yy)
	{
		x = xx;
		y = yy;
	};

	float x, y;
};
#define RGB_TO_INTENSITY(R,G,B)  ( 0.299*R + 0.587*G + 0.144*B)


class ImpBrush
{
protected:
	ImpBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL);

public:
	// The implementation of your brush should realize these virtual functions
	virtual void BrushBegin(const Point source, const Point target) = 0;
	virtual void BrushMove(const Point source, const Point target) = 0;
	virtual void BrushEnd(const Point source, const Point target) = 0;

	// according to the source image and the position, determine the draw color
	void SetColor(const Point source);

	// get Doc to communicate with it
	ImpressionistDoc *GetDocument(void);
	void initReverseData();

	// Return the name of the brush (not used in this version).
	char *BrushName(void);

	static int c_nBrushCount;	  // How many brushes we have,
	static ImpBrush **c_pBrushes; // and what they are.

	GLfloat alpha = 0;

private:
	ImpressionistDoc *m_pDoc;

	// Brush's name (not used in this version).
	char *m_pBrushName;
};

#endif