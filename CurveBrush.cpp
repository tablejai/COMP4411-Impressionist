//
// CurveBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CurveBrush.h"
#include <iostream>
#include <random>
#include  "GlobalFunction.h"
using namespace std;

extern float frand();

CurveBrush::CurveBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}


double CurveBrush::vectorDistance(const vector<int>& v1, const vector<int>& v2) {
	int dist = 0;
	for (int i = 0;i < v1.size();i++) {
		dist += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	}
	maxStrokeLength = 10;
	minStrokeLength = 6;
	return sqrt(dist);
}
void CurveBrush::BrushBegin(const Point source, const Point target)
{

	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	size = pDoc->getSize();
	alpha = pDoc->getAlpha();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initReverseData();
	glPointSize((float)size);
	BrushMove(source, target);
}
void CurveBrush::updateReverData() {
	/*for (int i = -size;i < size;i++) {

	}*/
}

BrushStroke CurveBrush::PaintStroke(int targetx, int targety, unsigned char* canvas, int w, int h, int radius, unsigned char* target, float kernel) {

	vector<unsigned char> color(4, 0);
	BrushStroke paintstroke;
	int maxStrokeLength = 10;
	int minStrokeLength = 5;
	if (maxStrokeLength == 0)
		maxStrokeLength = 1;
	vector<Point> points(maxStrokeLength, { 0,0 });
	vector<Point> pointsChange(maxStrokeLength, { 0,0 });
	paintstroke.color.push_back(Map4(target, 4 * targetx, targety, w));
	paintstroke.color.push_back(Map4(target, 4 * targetx + 1, targety, w));
	paintstroke.color.push_back(Map4(target, 4 * targetx + 2, targety, w));
	paintstroke.color.push_back(Map4(target, 4 * targetx + 3, targety, w));
	Point pts = { targetx, targety };
	points[0] = pts;
	paintstroke.pts.push_back(points[0]);
	for (int i = 1;i < maxStrokeLength;i++) {
		if (!(points[i - 1].x - 2 >= 0 && points[i - 1].x + 2 < w && points[i - 1].y - 2 >= 0 && points[i - 1].y + 2 < h))
			return paintstroke;
		vector<double> GXY = { 255 * getGradientX(points[i - 1],target,w,h), 255 * getGradientY(points[i - 1],target,w,h) };
		if (radius * sqrt(GXY[0] * GXY[0] + GXY[1] * GXY[1]) >= 1) {
			pointsChange[i] = Point{ (int)-GXY[1],(int)GXY[0] };
			if (i > 1 && pointsChange[i - 1].x * pointsChange[i].x + pointsChange[i - 1].y * pointsChange[i].y < 0) {
				pointsChange[i] = { (int)(-pointsChange[i].x),(int)(-pointsChange[i].y) };
			}
			pointsChange[i] = Point{ (int)(kernel * pointsChange[i].x + (1 - kernel) * pointsChange[i - 1].x), (int)(kernel * pointsChange[i].y + (1 - kernel) * pointsChange[i - 1].y) };
		}
		else {
			if (i > 1) {
				pointsChange[i] = Point{ (int)(pointsChange[i - 1].x),(int)(pointsChange[i - 1].y) };
			}
			else {
				return paintstroke;
			}

		}
		int xcahnge = pointsChange[i].x;
		int ychange = pointsChange[i].y;
		points[i] = Point{ (int)(points[i - 1].x + radius * xcahnge / sqrt(xcahnge * xcahnge + ychange * ychange)), (int)(points[i - 1].y + radius * ychange / sqrt(xcahnge * xcahnge + ychange * ychange)) };
		if (i > minStrokeLength) {
			if (4 * points[i].x < 0 || 4 * points[i].x >= 4 * (w - 2) || points[i].y >= h || points[i].y < 0) {
				return paintstroke;
			}
			try {
				vector<int> v1(3, 0);
				vector<int> v2(3, 0);
				v1[0] = Map4(canvas, 4 * points[i].x, points[i].y, w);
				v1[1] = Map4(canvas, 4 * points[i].x + 1, points[i].y, w);
				v1[2] = Map4(canvas, 4 * points[i].x + 2, points[i].y, w);
				v2[0] = Map4(target, 4 * points[i].x, points[i].y, w);
				v2[1] = Map4(target, 4 * points[i].x + 1, points[i].y, w);
				v2[2] = Map4(target, 4 * points[i].x + 2, points[i].y, w);
				vector<int> v3(3, 0);
				v3[0] = (UINT)color[0];
				v3[1] = (UINT)color[1];v3[2] = (UINT)color[2];
				if (vectorDistance(v1, v2) < vectorDistance(v1, v3)) {

					return paintstroke;
				}
			}
			catch (exception e) {
				cout << 4 * points[i].x + 2 << "," << points[i].y << w << " " << h << endl;
			}

		}
		paintstroke.pts.push_back(points[i]);
	}
	//cout << targetx << "||" << targety << endl;
	return paintstroke;
}



void CurveBrush::BrushMove(const Point source, const Point target)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawBuffer(GL_BACK);
	auto seed = mt19937{ random_device()() };
	mt19937 mt(seed);
	if (bmode == NORMALMODE)
		glPointSize((float)size);
	else
		glPointSize((float)size + ((int)mt()) % 4);
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int targetx = target.x;
	int targety = target.y;
	float radius = 2;
	float fc = 1.0f;
	BrushStroke b = PaintStroke(targetx, targety, dlg->m_paintView->rgbaBrush, dlg->m_paintView->m_nDrawWidth, dlg->m_paintView->m_nDrawHeight, radius, dlg->m_paintView->rgbaBitMap, fc);
	for (auto& pt : b.pts) {
		//	glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		int colorrand = (abs((int)mt()) % 2);
		float addr = 0;
		float addg = 0;
		float addb = 0;
		float red = ((float)(UINT)b.color[0] / 225.0f + addr > 1.0f) ? 1 : (float)(UINT)b.color[0] / 225.0f + addr;
		float green = ((float)(UINT)b.color[1] / 225.0f + addg > 1.0f) ? 1 : (float)(UINT)b.color[1] / 225.0f + addg;
		float blue = ((float)((UINT)b.color[2]) / 225.0f + addb > 1.0f) ? 1 : (float)(UINT)b.color[2] / 225.0f + addb;
		glColor4f(red, green, blue, alpha);
		glVertex2d(pt.x, pt.y);
		glEnd();
		//	glDisable(GL_POINT_SMOOTH);


	}
	cout << "drawing" << b.pts.size() << endl;

}
void CurveBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

