//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include "linebrush.h"
#include "GlobalFunction.h"
#include <iostream>
#include <random>
#include "BrushStroke.h"
#include  <unordered_map>
#include "math.h"
using namespace std;




#define LEFT_MOUSE_DOWN		1

#define LEFT_MOUSE_DRAG		2

#define LEFT_MOUSE_UP		3

#define RIGHT_MOUSE_DOWN	4

#define RIGHT_MOUSE_DRAG	5

#define RIGHT_MOUSE_UP		6



#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6
#define AUTO_PAINT          7

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
Point	coord;
Point	oldcoord;
Point mouseVec;
#define setParameter(mode,factorR,factorc,alpha,factorg,t,mx,mn,jr,jg,jb)  PaintParameter{mode,factorR,factorc,factorg,alpha,t, jr, jg, jb, mx, mn}
PaintlyMode mode;

//PaintParameter{ mode = Mode,blurrSize = factorR,fc = factorc,fg = factorg,a = alpha,threshold = t, maxStrokeLength = mx,minStrokeLength = mn, jr = kr,jg = kg,jb = kb };


extern PaintParameter paintModes[4] = {
	setParameter(Impressionist,0.5f,1,1,1,100,16,4,0,0,0),
	setParameter(Expressionist,0.5f,0.25f,0.7f,1,50,16,10,0,0,0),
    setParameter(Pointillist,0.5f,0.25f,0.5f,1,200,16,4,0.3,0.3,0.3),
	setParameter(Psychedelic,0.5f,0.5f,0.7f,1,50,16,10,0,0,0)
};
int  radiusNum = 3;
PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{


	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	initPaint = false;
	gradientMode = DEFUALT;
	radiusNum = 2;
	int r1 = 2;
	brushRadii = vector<int>(radiusNum,0);
	brushRadii[0] = r1;
	for (int i =1;i <radiusNum ;i++){
		brushRadii[i] = 2*brushRadii[i-1];
	}
	sort(brushRadii.begin(), brushRadii.end(), greater<int>());
	paintlymode = Impressionist;
	updatePaintlyPara();
}
void PaintView::updatePaintlyPara() {
	if (paintlymode != Custom) {
		blurrSize = paintModes[paintlymode].blurrSize;
		fc = paintModes[paintlymode].fc;
		fg = paintModes[paintlymode].fg;
		a = paintModes[paintlymode].a;
		threshold = paintModes[paintlymode].threshold;
		maxStrokeLength = paintModes[paintlymode].maxStrokeLength;
		minStrokeLength = paintModes[paintlymode].minStrokeLength;
	}
}
void PaintView::INIT_RGBA(GLvoid* data, unsigned char*& RGBA, int w, int h, int a) {

	if (w > 0 && h > 0) {
		RGBA = new unsigned char[w * h * 4];
		memset(RGBA, 0, w * h * 4);
		for (int j = 0;j < h;j++) {
			int id = 0;
			for (int i = 0; i < w * 4;i++) {
				int index = i % 4;
				if (index < 3)
					Map4(RGBA, i, j, w) = Map(data, id, j, w);
				else {
					Map4(RGBA, i, j, w) = (char)a;
					id--;
				}
				id++;
			}
		}
	}
	cout << w << "," << h << endl;
	cout << "rgb to rgba" << endl;

}



void PaintView::randomBrushDraw() {
	
	



}

unsigned char* PaintView::getrgbaBrush() {
	return rgbaBrush;
}
GradientMode PaintView::getGradientMode() {
	return  gradientMode;
}
void PaintView::setGradientMode(GradientMode mode) {
	gradientMode = mode;
}
void PaintView::autoPaint() {
	glDrawBuffer(GL_BACK);
	auto seed = mt19937{ random_device()() };
	mt19937 mt(seed);
	int length;
	m_pDoc->clearCanvas();

	SynchronizeContentRGBA(rgbaBrush, m_pUndoBitstart);
	RestorePreviousDataRGBA(rgbaBrush, GL_BACK);
	vector<Point> points;
	m_pDoc->m_pCurrentBrush->setBrushMode(RANDOMMODE);
	for (int i = 0;i < m_nDrawHeight;i++) {
		for (int j = 0;j < m_nDrawWidth;j++) {
			points.push_back(Point{ j,i });
		}
	}
	random_shuffle(points.begin(), points.end());
	for (int k = 0;k < points.size();k++) {
		int i = points[k].y, j = points[k].x;
		Point source(j + m_nStartCol, m_nEndRow - i);
		Point target(j, m_nWindowHeight - i);
		coord.x = j;
		coord.y = i;
		m_pDoc->m_pCurrentBrush->BrushBegin(source, target);//should draw to back
		m_pDoc->m_pCurrentBrush->BrushMove(source, target);//should draw to back
		m_pDoc->m_pCurrentBrush->BrushEnd(source, target);//should draw to back
	}
	std::cout << m_pDoc->m_pCurrentBrush->BrushName() << "bursh nm" << endl;

	SavePreviousDataRGBA(rgbaBrush, GL_BACK);
	clearColorBuffer(GL_BACK);
	AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
	AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
	glFlush();
	glDrawBuffer(GL_BACK);
	this->redraw();
	m_pDoc->m_pCurrentBrush->setBrushMode(NORMALMODE);


}

void PaintView::drawPaintly() {
	glDrawBuffer(GL_BACK);
	auto seed = mt19937{ random_device()() };
	mt19937 mt(seed);
	int length;
	m_pDoc->clearCanvas();
	SynchronizeContentRGBA(rgbaBrush, m_pUndoBitstart);
	int r1 = 2;
	brushRadii = vector<int>(radiusNum, 0);
	brushRadii[0] = r1;
	for (int i = 1;i < radiusNum;i++) {
		brushRadii[i] = 2 * brushRadii[i - 1];
	}
	sort(brushRadii.begin(), brushRadii.end(), greater<int>());
	Paint(true);
	SavePreviousDataRGBA(rgbaBrush, GL_BACK);
	clearColorBuffer(GL_BACK);
	AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
	AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
	glFlush();
	glDrawBuffer(GL_BACK);
	this->redraw();
	m_pDoc->m_pCurrentBrush->setBrushMode(NORMALMODE);
}
void PaintView::blurrPointOrigin(unsigned char * ptr,const Point source,const Point target,int paintWidth,int paintHeight,const vector<vector<double>>& kernel) {
	int rSum = 0, gSum = 0,bSum = 0;
	int kernelSize = kernel.size();
	for (int i = 0; i < kernelSize; i++) {
		for (int j = 0; j < kernelSize; j++) {
			int srcX = source.x + i - kernelSize / 2;
			int srcY = source.y + j - kernelSize / 2;
			//	cout << srcX << "," << srcY << endl;
			if (srcX >= (paintWidth - 1 - 1) || srcX < 0 || srcY >= (paintHeight - 1 - 1) || srcY < 0) {
				continue;
			}
			GLubyte srcColor[3];
			memcpy(srcColor, m_pDoc->GetOriginalPixel(Point(srcX, srcY)), 3);

			float kernelVal = kernel[i][j];
			rSum += srcColor[0] * kernelVal;
			gSum += srcColor[1] * kernelVal;
			bSum += srcColor[2] * kernelVal;
			KernelSetColor(ptr, target, rSum, gSum, bSum, 255);

		}
	}


}
void PaintView::blurrPoint(unsigned char* sourceptr, unsigned char* ptr, const Point source, const Point target, int paintWidth, int paintHeight, const vector<vector<double>>& kernel) {
	int rSum = 0, gSum = 0, bSum = 0;
	int kernelSize = kernel.size();
	for (int i = 0; i < kernelSize; i++) {
		for (int j = 0; j < kernelSize; j++) {
			int srcX = target.x + i - kernelSize / 2;
			int srcY = target.y + j - kernelSize / 2;
			//	cout << srcX << "," << srcY << endl;
			if (srcX >= (paintWidth - 1 - 1) || srcX < 0 || srcY >= (paintHeight - 1 - 1) || srcY < 0) {
				continue;
			}
			GLubyte srcColor[3];
			srcColor[0] = Map4(sourceptr,4* srcX, srcY, paintWidth);
			srcColor[1] = Map4(sourceptr, 4 * srcX+1, srcY, paintWidth);
			srcColor[2] = Map4(sourceptr, 4 * srcX+2, srcY, paintWidth);

			float kernelVal = kernel[i][j];
			rSum += srcColor[0] * kernelVal;
			gSum += srcColor[1] * kernelVal;
			bSum += srcColor[2] * kernelVal;
			KernelSetColor(ptr, target, rSum, gSum, bSum, 255);

		}
	}


}
void PaintView::KernelSetColor(unsigned char* ptr,const Point target, const int r, const int g, const int b,const int alpha) {
	ImpressionistDoc* pDoc = m_pDoc;
	ImpressionistUI* dlg = pDoc->m_pUI;
	int w = pDoc->m_nPaintWidth;
	int h = pDoc->m_nPaintHeight;
	Fl_Color_Chooser* colorChooser = dlg->m_ColorChooser;
	int r_mult = colorChooser->r();
	int g_mult = colorChooser->g();
	int b_mult = colorChooser->b();
	ptr[(target.x + target.y * w) * 4] = min(max(r, 0), 255) * r_mult;
	ptr[(target.x + target.y * w) * 4 + 1] = min(max(g, 0), 255) * g_mult;
	ptr[(target.x + target.y * w) * 4 + 2] = min(max(b, 0), 255) * b_mult;
	ptr[(target.x + target.y * w) * 4 + 3] = alpha;
}

void PaintView::blurr(unsigned char * sourceMap,unsigned char * targetMap,float blursize) {
	/*vector<vector<float>> kernel = { {1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0},
			   {1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0},
			   {1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0},
				{1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0},
				{1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0}, };*/
	double sigma = blursize;
	int W = 5;
	vector<vector<double>> kernel(W,vector<double>(W,0));
	double mean = W / 2;
	double sum = 0.0; // For accumulating the kernel values
	for (int x = 0; x < W; ++x)
		for (int y = 0; y < W; ++y) {
			kernel[x][y] = exp(-0.5 * (pow((x - mean) / sigma, 2.0) + pow((y - mean) / sigma, 2.0)))
				/ (2 * M_PI * sigma * sigma);
			sum += kernel[x][y];
		}

	// Normalize the kernel
	for (int x = 0; x < W; ++x)
		for (int y = 0; y < W; ++y)
			kernel[x][y] /= sum;
	int kernelSize = kernel.size();
	for (int j = 0;j < m_nDrawHeight;j++) {
		for (int i = 0;i < m_nDrawWidth;i++) {
			Point source(i + m_nStartCol, m_nEndRow - j);
			Point target(i, m_nDrawHeight - j);
			if (sourceMap == nullptr)
				blurrPointOrigin(targetMap, source, target, m_nDrawWidth, m_nDrawHeight, kernel);
			else
			blurrPoint(sourceMap,targetMap, target,target, m_nDrawWidth, m_nDrawHeight,kernel);
		}
	}
}


double PaintView::vectorDistance(const vector<int>& v1, const vector<int>& v2) {
	int dist = 0;
	for (int i = 0;i<v1.size();i++) {
		dist += (v1[i] - v2[i])*(v1[i] - v2[i]);
	}
	return sqrt(dist);
}

BrushStroke PaintView::PaintStroke(int targetx, int targety, unsigned char* canvas,int w, int h,int radius, unsigned char* target ,float kernel) {

vector<unsigned char> color(4,0);
BrushStroke paintstroke;

//init K
//add
if (maxStrokeLength == 0)
maxStrokeLength = 1;
 vector<Point> points(maxStrokeLength, { 0,0 });
 vector<Point> pointsChange(maxStrokeLength, { 0,0 });
 paintstroke.color.push_back(Map4(target, 4 * targetx, targety, w));
 paintstroke.color.push_back(Map4(target, 4 * targetx+1, targety, w));
 paintstroke.color.push_back(Map4(target, 4 * targetx+2, targety, w));
 paintstroke.color.push_back(Map4(target, 4 * targetx+3, targety, w));

Point pts = { targetx, targety};
points[0] = pts;
paintstroke.pts.push_back(points[0]);
for (int i = 1;i < maxStrokeLength;i++) {
	if (!(points[i - 1].x-2 >= 0 && points[i - 1].x+2 < w && points[i - 1].y - 2 >= 0 && points[i - 1].y + 2 < h))
		return paintstroke;
	vector<double> GXY = {255* getGradientX(points[i-1],target,w,h), 255*getGradientY(points[i-1],target,w,h)};
	if (radius*sqrt(GXY[0]* GXY[0]+ GXY[1]* GXY[1])>=1 ) {
		pointsChange[i] = Point{ (int)-GXY[1],(int)GXY[0] };
		if (i > 1 && pointsChange[i - 1].x * pointsChange[i].x + pointsChange[i - 1].y * pointsChange[i].y < 0) {
			pointsChange[i] = { (int)(-pointsChange[i].x),(int)(-pointsChange[i].y)};
		}
		pointsChange[i] = Point{ (int)(kernel* pointsChange[i].x+(1-kernel)* pointsChange[i-1].x ), (int)(kernel * pointsChange[i].y + (1 - kernel) * pointsChange[i-1].y )};
	}
	else {
		if (i > 1) {
		pointsChange[i] = Point{ (int)(pointsChange[i - 1].x),(int)(pointsChange[i - 1].y)};
		}
		else{
			return paintstroke;
		}
		
	}
	int xcahnge = pointsChange[i].x;
	int ychange = pointsChange[i].y;
	points[i] = Point{ (int)(points[i - 1].x + radius * xcahnge / sqrt(xcahnge * xcahnge + ychange * ychange)), (int)(points[i - 1].y + radius * ychange / sqrt(xcahnge * xcahnge + ychange * ychange)) };
	if (i > minStrokeLength ) {
		if (4 * points[i].x<0||4 * points[i].x >= 4*(w-2) || points[i].y >= h||points[i].y<0) {
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
			vector<int> v3(3,0);
			v3[0] = (UINT)color[0];
			v3[1] = (UINT)color[1];v3[2]= (UINT)color[2];
			if (vectorDistance(v1, v2) < vectorDistance(v1, v3)) {

				return paintstroke;
			}
		}
		catch(exception e){
			cout << 4*points[i].x + 2 << "," << points[i].y <<w<<" "<<h << endl;
		}

	}
	paintstroke.pts.push_back(points[i]);
}
//cout << targetx << "||" << targety << endl;
return paintstroke;
}






void PaintView::Paint(bool fristFrame){
	GLvoid* src = m_pPaintBitstart;

	bool refersh = fristFrame;
	vector<unsigned char*> Ips;
	unsigned char* canvas = new unsigned char[m_nDrawWidth * m_nDrawHeight * 4];
	unsigned char* temp = new unsigned char[m_nDrawWidth * m_nDrawHeight * 4];
	unsigned char* blurrtemp = new unsigned char[m_nDrawWidth * m_nDrawHeight * 4];

	memset(canvas, m_nDrawWidth* m_nDrawHeight*4,0);
	cout << brushRadii.size()<<"size";
	unsigned char* blurred;
	blurred = new unsigned char[m_nDrawWidth * m_nDrawHeight * 4];
//	cout << m_nDrawWidth << "," << m_nDrawWidth << endl;
	RestorePreviousDataRGBA(canvas, GL_BACK);
	RGB_TO_RGBA(m_pDoc->m_ucBitmap,temp, m_nDrawWidth, m_nDrawHeight,255);
	//blurr(temp, blurrtemp, 0.5f);

  for(auto& radius:brushRadii){
	  memset(blurred, m_nDrawWidth * m_nDrawHeight * 4, 0);
	 blurr(temp, blurred, blurrSize * radius);
	//cout << "radius" << radius << endl;
	float grid = radius;
	RestorePreviousDataRGBA(canvas, GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for (int j = grid / 2;j < m_nDrawHeight;j += grid) {
			for (int i = grid / 2;i < m_nDrawWidth;i += grid) {
				double areaError = 0;
				int mxdist = INT_MIN;
				int targetx = 0;
				int targety = 0;
				glClear(GL_DEPTH_BUFFER_BIT);
				if (j - grid / 2 >= 0 && j + grid / 2 < m_nDrawHeight - 1 && i - grid / 2 >= 0 && i + grid / 2 < m_nDrawWidth - 1) {
					for (int ky = j - grid / 2;ky <= j + grid / 2;ky++) {
						for (int k = i - grid / 2;k <= i + grid / 2;k++) {
							vector<int> rgb = { Map4(blurred,4 * k,ky,m_nDrawWidth),
							Map4(blurred,4 * k + 1,ky,m_nDrawWidth),
							Map4(blurred,4 * k + 2,ky,m_nDrawWidth) };
							vector<int> rgb2 = { Map4(canvas,4 * k,ky,m_nDrawWidth),
							Map4(canvas,4 * k + 1,ky,m_nDrawWidth),
							Map4(canvas,4 * k + 2,ky,m_nDrawWidth) };
							double dist = vectorDistance(rgb, rgb2);
							areaError += dist;
							if (dist > mxdist)
							{
								targetx = k;
								targety = ky;
								mxdist = dist;
							}
						}
					}
					if (refersh || areaError > threshold) {
						//cout << "in here"<< areaError << endl;
						//cout << "xxxxxx" << endl;i
						if (targetx == 0 && targety == 0)
							cout << i << "," << j << "," << targetx << "," << targety << endl;
						BrushStroke b = PaintStroke(targetx, targety, canvas, m_nDrawWidth, m_nDrawHeight, radius, blurred, fc);
						auto seed = mt19937{ random_device()() };
						mt19937 mt(seed);
						for (auto& pt : b.pts) {
							glPointSize((float)radius);
						//	glEnable(GL_POINT_SMOOTH);
							glBegin(GL_POINTS);
							int colorrand = (abs((int)mt()) % 2);
							float addr=0;
							float addg=0;
							float addb=0; 
							if (paintlymode != CUSTOM) {
								addg = colorrand * paintModes[paintlymode].jg;
								addr = colorrand * paintModes[paintlymode].jr;
								addb = colorrand * paintModes[paintlymode].jb;
							}
							
							float red = ((float)(UINT)b.color[0] / 225.0f + addr > 1.0f) ? 1 : (float)(UINT)b.color[0] / 225.0f + addr ;
							float green = ((float)(UINT)b.color[1] / 225.0f + addg > 1.0f) ? 1 : (float)(UINT)b.color[1] / 225.0f + addg ;
							float blue = ((float)((UINT)b.color[2])/ 225.0f + addb >1.0f) ? 1 : (float)(UINT)b.color[2] / 225.0f + addb ;
							glColor4f(red, green, blue, a);
							glVertex3d(pt.x, pt.y, abs((int)mt())%radiusNum);
							glEnd();
						//	glDisable(GL_POINT_SMOOTH);


						}
					}
				}
			}
			refersh = false;
		}
		SavePreviousDataRGBA(canvas, GL_BACK);


	}
  //RestorePreviousDataRGBA(temp, GL_BACK);
  //AddPreviousDataRGBA(canvas, GL_BACK,NONCOVER);
  
	delete[]blurred;





}

void PaintView::draw()
{
#ifndef MESA
		// To avoid flicker on some machines.
		glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA
		static Point rightClickDownPt;
		if (!valid())
		{

			glClearColor(0.7f, 0.7f, 0.7f, 1.0);

			// We're only using 2-D, so turn off depth 
			glDisable(GL_DEPTH_TEST);

			ortho();

			glClear(GL_COLOR_BUFFER_BIT);
		}
		Point scrollpos;// = GetScrollPosition();
		scrollpos.x = 0;
		scrollpos.y = 0;
		m_nWindowWidth = w();
		m_nWindowHeight = h();

		int drawWidth, drawHeight;
		drawWidth = min(m_nWindowWidth, m_pDoc->m_nPaintWidth);
		drawHeight = min(m_nWindowHeight, m_pDoc->m_nPaintHeight);

		int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
		if (startrow < 0) startrow = 0;

		m_pPaintBitstart = m_pDoc->m_ucPainting +
			3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);
		m_pUndoBitstart = m_pDoc->m_undoBitMap + 4 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

		m_nDrawWidth = drawWidth;
		m_nDrawHeight = drawHeight;
		m_nStartRow = startrow;
		m_nEndRow = startrow + drawHeight;
		m_nStartCol = scrollpos.x;
		m_nEndCol = m_nStartCol + drawWidth;
		if (!rgbaBitMap&& m_pDoc->m_rgbaBitMap) { //init load new image 
			rgbaBitMap = m_pDoc->m_rgbaBitMap + 4 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);
			clearColorBuffer(GL_BACK);
			RGB_TO_RGBA(m_pDoc->m_ucBitmap, rgbaBitMap, m_pDoc->m_nWidth, m_pDoc->m_nHeight, backgroundalpha * 255);
			AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
		}
		else if(rgbaBitMap)
			RGB_TO_RGBA(m_pDoc->m_ucBitmap, rgbaBitMap, m_pDoc->m_nWidth, m_pDoc->m_nHeight, backgroundalpha * 255);
		if (!rgbaBrush && m_pDoc->m_rgbaBrush) {
			rgbaBrush = m_pDoc->m_rgbaBrush + 4 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);
		}
		if (m_pDoc->m_ucPainting && m_pDoc->m_rgbaBitMap && m_pDoc->m_rgbaBrush && !isAnEvent)
		{
			refreshPaintView();
		}
		if (m_pDoc->m_ucPainting && m_pDoc->m_rgbaBitMap && m_pDoc->m_rgbaBrush && isAnEvent)
		{
			isAnEvent = 0;
			Point source(coord.x + m_nStartCol, m_nEndRow - coord.y);
			Point target(coord.x, m_nWindowHeight - coord.y);
			switch (eventToDo)
			{
			case LEFT_MOUSE_DOWN:
				SynchronizeContentRGBA(rgbaBrush, m_pUndoBitstart);
			//	clearColorBuffer(GL_BACK);
				RestorePreviousDataRGBA(rgbaBrush,GL_BACK);
				if (source.y < 0 || source.x > m_nWindowWidth) return;
				m_pDoc->m_pCurrentBrush->BrushBegin(source, target);//should draw to back
				SavePreviousDataRGBA(rgbaBrush, GL_BACK);
				clearColorBuffer(GL_BACK);
				AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
				AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
				break;
			case LEFT_MOUSE_DRAG:
				if (source.y < 0 || source.x > m_nWindowWidth) return;
				//clearColorBuffer(GL_BACK);
				//AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
				RestorePreviousDataRGBA(rgbaBrush, GL_BACK);
				m_pDoc->m_pCurrentBrush->BrushMove(source, target);
				SavePreviousDataRGBA(rgbaBrush, GL_BACK);
				clearColorBuffer(GL_BACK);
				AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
				AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);

				break;
			case LEFT_MOUSE_UP:
				//clearColorBuffer(GL_BACK);
				//AddPreviousDataRGBA(rgbaBrush, GL_BACK);
				RestorePreviousDataRGBA(rgbaBrush, GL_BACK);
				m_pDoc->m_pCurrentBrush->BrushEnd(source, target);
				SavePreviousDataRGBA(rgbaBrush, GL_BACK);
				clearColorBuffer(GL_BACK);
				AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
				AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
				break;

#define BRUSH_IS_STROKE (strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Lines")==0 || strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Scattered Lines")==0)
			case RIGHT_MOUSE_DOWN:
				if (m_pDoc->c_pStrokes == STROKE_SLIDER_OR_RIGHT_MOUSE && BRUSH_IS_STROKE)
					rightClickDownPt = source;
				break;
			case RIGHT_MOUSE_DRAG:

				//SavePreviousDataRGBA(rgbaBrush, GL_BACK);
				if (m_pDoc->c_pStrokes == STROKE_SLIDER_OR_RIGHT_MOUSE && BRUSH_IS_STROKE)
				{
					clearColorBuffer(GL_BACK);
					AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
					AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
					glBegin(GL_LINES);
					glVertex2d(rightClickDownPt.x, rightClickDownPt.y);
					glVertex2d(target.x, target.y);
				
				}
				glEnd();

				break;
			case RIGHT_MOUSE_UP:
				if (m_pDoc->c_pStrokes == STROKE_SLIDER_OR_RIGHT_MOUSE && BRUSH_IS_STROKE) {
					RestorePreviousDataRGBA(rgbaBrush, GL_BACK);
					{
						float angle;
						if ((target.x - rightClickDownPt.x) == 0) {
							angle = 0;
						}
						else {
#define PI 3.14159265358979
							angle = atan2((target.y - rightClickDownPt.y), (target.x - rightClickDownPt.x)) * 180 / PI;
#undef PI
						}
						ImpressionistUI* dlg = m_pDoc->m_pUI;
						dlg->m_nAngle = angle;
						std::cout << angle << std::endl;

						std::cout << m_pDoc->getAngle() << std::endl;
					}
					clearColorBuffer(GL_BACK);
					AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
					AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);

				}
				break;
#undef BRUSH_IS_STROKE
			case AUTO_PAINT:
				break;
			default:
				printf("Unknown event!!\n");
				break;
			}
		}
		if (drawstate == UNDO) {
			//clearColorBuffer(GL_BACK);
			SynchronizeContentRGBA(m_pUndoBitstart, rgbaBrush);
			clearColorBuffer(GL_BACK);
			AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
			AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
			drawstate = DO;
		}

		glFlush();

#ifndef MESA
		// To avoid flicker on some machines.
		glDrawBuffer(GL_BACK);
#endif // !MESA

}

void PaintView::refreshPaintView() {
	//clearColorBuffer(GL_BACK);
	RestorePreviousDataRGBA(rgbaBrush, GL_BACK);
	SavePreviousDataRGBA(rgbaBrush, GL_BACK);
	clearColorBuffer(GL_BACK);
	AddPreviousDataRGBA(rgbaBitMap, GL_BACK, NONCOVER);
	AddPreviousDataRGBA(rgbaBrush, GL_BACK, NONCOVER);
}


void PaintView::resetBackGround(unsigned char* ptr) {
	rgbaBitMap = ptr;
}
void PaintView::resetBrush(unsigned char* ptr) {
	rgbaBrush = ptr;
}
void PaintView::resetUndo(unsigned char* ptr) {
	m_pUndoBitstart = ptr;
}
void PaintView::updateBackGroundAlpha(float alpha) {
	backgroundalpha = alpha;
}


int PaintView::handle(int event)
{
if(m_pDoc->m_pCurrentBrush->bmode==NORMALMODE)
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		if (abs(coord.x - oldcoord.x) > 0 && abs(coord.y - oldcoord.y) > 0)
		{
			mouseVec.x = coord.x - oldcoord.x;
			mouseVec.y = coord.y - oldcoord.y;
			oldcoord = coord;

		}		
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		m_pDoc->m_pCursor->setpos(coord.x, coord.y);
		m_pDoc->m_pUI->m_origView->triggerupdate();
		redraw();
		if (abs(coord.x - oldcoord.x) >0 && abs(coord.y - oldcoord.y) > 0)
		{
			mouseVec.x = coord.x - oldcoord.x;
			mouseVec.y = coord.y - oldcoord.y;
			oldcoord = coord;

		}
		break;
	case FL_DRAG: {
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_DRAG;
		else
			eventToDo = LEFT_MOUSE_DRAG;
		isAnEvent = 1;
		if (coord.x > 0 && coord.x <= m_nWindowWidth && coord.y > 0 && coord.y <= m_nWindowHeight)
			m_pDoc->m_pCursor->setpos(coord.x, coord.y);
	
		m_pDoc->m_pUI->m_origView->triggerupdate();
		redraw();
		if (abs(coord.x - oldcoord.x) > 0 && abs(coord.y - oldcoord.y) > 0)
		{
			mouseVec.x = coord.x - oldcoord.x;
			mouseVec.y = coord.y - oldcoord.y;
			oldcoord = coord;

		}

		break;
	}
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		m_pDoc->m_pCursor->setpos(coord.x, coord.y);

		m_pDoc->m_pUI->m_origView->triggerupdate();
		redraw();
		if (abs(coord.x - oldcoord.x) > 0 && abs(coord.y - oldcoord.y) > 0)
		{
			mouseVec.x = coord.x - oldcoord.x;
			mouseVec.y = coord.y - oldcoord.y;
			oldcoord = coord;

		}

		break;

	case FL_MOVE: {

		coord.x = Fl::event_x();

		coord.y = Fl::event_y();

		if (coord.x > 0 && coord.x <= m_nWindowWidth && coord.y > 0 && coord.y <= m_nWindowHeight)

			m_pDoc->m_pCursor->setpos(coord.x, coord.y);
		/*DEBUG start*/
		Point target(coord.x, m_nWindowHeight - coord.y);
		glReadBuffer(GL_FRONT);

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
		unsigned char b[3];
		glReadPixels(target.x,
			target.y,
			1,
			1,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			b);
		//cout << (int)b[0] << "," << (int)b[1] << "," << (int)b[2] << "|" << endl;
		/*DEBUG end*/
		m_pDoc->m_pUI->m_origView->triggerupdate();
		if (abs(coord.x - oldcoord.x) > 1 && abs(coord.y - oldcoord.y) > 1)
		{
			mouseVec.x = coord.x - oldcoord.x;
			mouseVec.y = coord.y - oldcoord.y;
			oldcoord = coord;

		}
		break;}
	default:
		return 0;
		break;

	}


	return 1;
}


void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}
void PaintView::undo() {
	if (m_pUndoBitstart != nullptr) {
		cout << "undo bit" << endl;
		drawstate = UNDO;
		redraw();
		return;
	}
	//cout << "it did not return " << endl;

}
//void PaintView::clearView() {
//	drawstate = CLEAR;
//	m_pPaintBitstart = nullptr;
//	m_pUndoBitstart = nullptr;
//	rgbaBitMap = nullptr;
//	rgbaBrush = nullptr;
//	redraw();
//}
void PaintView::setstate(ReDrawState state) {
	drawstate = state;
}
void PaintView::SaveCurrentContent(GLenum mode)
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(mode);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
			  m_nDrawWidth, 
			  m_nDrawHeight, 
			  GL_RGB, 
			  GL_UNSIGNED_BYTE,
		  m_pPaintBitstart);

}

void PaintView::SavePreviousData(GLvoid* data) {
	if (data == nullptr) {
		cout << "isnull" << endl;
		return;
	}
	cout << "save previous data" << endl;
	glReadBuffer(GL_FRONT);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
	glReadPixels(0,
		m_nWindowHeight - m_nDrawHeight,
		m_nDrawWidth,
		m_nDrawHeight,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		data);

}
void PaintView::SavePreviousDataRGBA(GLvoid* data,GLenum mode) {
	if (data == nullptr) {
		cout << "isnull" << endl;
		return;
	}
	//cout << "save previous data [RGBA]" << endl;
	glReadBuffer(mode);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
	glReadPixels(0,
		m_nWindowHeight - m_nDrawHeight,
		m_nDrawWidth,
		m_nDrawHeight,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data);

}

void PaintView::RestorePreviousData(GLvoid* data) {
	glDrawBuffer(GL_BACK);
//	cout << "restore previous data" << endl;
	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2i(0, m_nWindowHeight - m_nDrawHeight);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
	glDrawPixels(m_nDrawWidth,
		m_nDrawHeight,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		data);
}





void PaintView::RestorePreviousDataRGBA(GLvoid* data,GLenum mode) {
	glDrawBuffer(GL_BACK);
	//glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2i(0, m_nWindowHeight - m_nDrawHeight);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
	glDrawPixels(m_nDrawWidth,
		m_nDrawHeight,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data);
}


void PaintView::AddPreviousDataRGBA(GLvoid* data, GLenum mode, PaintMode ptMode) {
	glDrawBuffer(mode);
	if(data!=nullptr)
	if (ptMode == COVER) {
		glRasterPos2i(0, m_nWindowHeight - m_nDrawHeight);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
		glDrawPixels(m_nDrawWidth,
			m_nDrawHeight,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data);
	}
	else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPointSize(1);
		glBegin(GL_POINTS);
		for (int j = 0;j < m_pDoc->m_nPaintHeight;j++) {
			for (int i = 0;i < m_pDoc->m_nPaintWidth;i++) {
			GLbyte r = Map4(data, i*4, j, m_pDoc->m_nPaintWidth);
			GLbyte g = Map4(data, i * 4+1, j, m_pDoc->m_nPaintWidth);
			GLbyte b = Map4(data, i * 4 + 2, j, m_pDoc->m_nPaintWidth);
			GLbyte a = Map4(data, i * 4 + 3, j, m_pDoc->m_nPaintWidth);
			glColor4ub(r,g,b,a);
			glVertex2d(i,j);
			}
		}
		glEnd();
		glDisable(GL_BLEND);

	}
}
void PaintView::clearColorBuffer( GLenum buffer) {
	glDrawBuffer(buffer);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0,0,0,0); //dont put 0
	glClear(GL_COLOR_BUFFER_BIT);
	//glDisable(GL_BLEND);


}

void PaintView::RestoreContent(GLenum mode)
{
	glDrawBuffer(mode);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);
}
void PaintView::SynchronizeContent(GLvoid* source, GLvoid* target) {
for (int i = 0;i < m_pDoc->m_nPaintWidth*3;i++) {
	for (int j = 0;j < m_pDoc->m_nPaintHeight;j++) {
		Map(target,i,j,m_pDoc->m_nPaintWidth) = Map(source,i,j,m_pDoc->m_nPaintWidth);
	}
}
}
void PaintView::SynchronizeContentRGBA(GLvoid* source, GLvoid* target) {
	for (int i = 0;i < m_pDoc->m_nPaintWidth * 4;i++) {
		for (int j = 0;j < m_pDoc->m_nPaintHeight;j++) {
			Map4(target, i, j, m_pDoc->m_nPaintWidth) = Map4(source, i, j, m_pDoc->m_nPaintWidth);
		}
	}
}

