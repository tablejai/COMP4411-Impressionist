#include "ImpressionistDoc.h"
#include <FL/gl.h>
#include "GlobalFunction.h"
double meanfilter(const Point source, unsigned char* data,int width, int height) {
	double meanintensity = 0;
	GLubyte color[3];
	// did not handle 5 edge
	for (int i = -1;i < 2;i++) {
		for (int j = -1;j < 2;j++) {
			Point temp;
			temp.x = source.x + i;
			temp.y = source.y + j;
	
			color[0] = Map4(data, 4 * temp.x, temp.y, width);
			color[1] = Map4(data, 4 * temp.x + 1, temp.y, width);
			color[2] = Map4(data, 4 * temp.x + 2, temp.y, width);
		
			meanintensity += RGB_TO_INTENSITY(color[0], color[1], color[2]);
		}
	}
	return meanintensity / 9;

}
double meanfilter(const Point source, unsigned char* data, int width, int height,int incre) {
	double meanintensity = 0;
	GLubyte color;
	// did not handle 5 edge
	for (int i = -1;i < 2;i++) {
		for (int j = -1;j < 2;j++) {
			Point temp;
			temp.x = source.x + i;
			temp.y = source.y + j;

			color = Map4(data, 4 * temp.x+incre, temp.y, width);
			meanintensity += color;
		}
	}
	return meanintensity / 9;

}
double getGradientX(const Point source, unsigned char* data, int width, int height) {
	Point tempR(source.x + 1, source.y);
	Point tempD(source.x, source.y - 1);
	double intensity = meanfilter(source,data,width,height);
	double intensity1 = meanfilter(tempR,data,width, height);
	double gradientx = (intensity1 - intensity) / 2;
	return gradientx;
}

double getGradientY(const Point source, unsigned char* data, int width, int height) {
	Point tempR(source.x + 1, source.y);
	Point tempD(source.x, source.y - 1);
	double intensity = meanfilter(source, data, width, height);
	double intensity2 = meanfilter(tempD, data, width, height);
	double gradienty= (intensity - intensity2) / 2;
	return gradienty;
}