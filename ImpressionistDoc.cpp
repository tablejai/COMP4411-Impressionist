// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "linebrush.h"
#include "scatterline.h"
#include "ImpBrush.h"
#include "circleBrush.h"
#include "scattercircle.h"
#include "scatterpoint.h"
#include "traingle.h"
#include "starbrush.h"
#include "BlurringBrush.h"
#include "SharpeningBrush.h"
#include "ImageCursor.h"
#include "AlphaMapBrush.h"
// Include individual brush headers here.
#include "PointBrush.h"
#include <iostream>
using namespace std;

#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }
void RGB_TO_RGBA(GLvoid* data, unsigned char* RGBA, int w, int h, int a) {
	if (w > 0 && h > 0) {
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


}

void RGBA_TO_RGB(GLvoid* data, unsigned char* RGB, int w, int h, int a) {
	if (w > 0 && h > 0) {
		for (int j = 0;j < h;j++) {
			int id = 0;
			for (int i = 0; i < w * 3;i++) {
				int index = i % 3;
				if (index == 2) {
					Map(RGB, i, j, w) = Map4(data, id, j, w);
					id++;
				}
				id++;
			}
		}
	}


}

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_undoBitMap = NULL;
	m_uAlphaMap = NULL;
	m_rgbaBitMap = NULL;
	m_rgbaBrush = NULL;
	m_ucGradeint = NULL;
	m_uctempBitmap1 = NULL;
	m_uctempBitmap2 = NULL;
	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatterPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatterLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatterCircleBrush( this, "Scattered Circles" );
	ImpBrush::c_pBrushes[BRUSH_STAR]
		= new StarBrush(this, "Star");
	ImpBrush::c_pBrushes[BRUSH_TRAINGLE]
		= new TraingleBrush(this, "Traingle");
	ImpBrush::c_pBrushes[BRUSH_ALPHAMAP]
		= new AlphaBrush(this, "Alpha Map Brush");
	ImpBrush::c_pBrushes[BRUSH_BLURRRING]
		= new BlurringBrush(this, "Blurring");
	ImpBrush::c_pBrushes[BRUSH_SHARPENING]
		= new SharpeningBrush(this, "Sharpening");
	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];
	char name[50] = "ImageCursor";
	m_pCursor = new ImageCursor{ this,name };

}
//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

void ImpressionistDoc::setStrokeType(int type)
{
	c_pStrokes = (StrokeDir)type;
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

int	ImpressionistDoc::getWidth() {
	return m_pUI->getWidth();
}
int	ImpressionistDoc::getAngle() {
	return m_pUI->getAngle();
}
GLfloat	ImpressionistDoc::getAlpha() {
	return m_pUI->getAlpha();
}
//void ImpressionistDoc::setWidth(int width) {
//	//m_pUI->setSize(
//}
//void	setSize(int size);
//void	setAngle(int angle);
//void	setAlpha(float alpha);
//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
void ImpressionistDoc::saveOldImage() {
	oldPaintWidth = m_nPaintWidth;
	oldPaintHeight = m_nPaintHeight;
	if (temp_m_ucPainting == nullptr)
		delete[] temp_m_ucPainting;
	temp_m_ucPainting = new unsigned char[oldPaintWidth * oldPaintHeight * 4];
	memcpy(temp_m_ucPainting, m_rgbaBrush, oldPaintWidth * oldPaintHeight * 4);
}

int ImpressionistDoc::loadGradientImage(char* name)
{
	unsigned char* data;
	int width, height;
	if ((data = readBMP(name, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}
	m_nGradientWidth = width;
	m_nGradientHeight = height;
	if (m_ucGradeint) delete[] m_ucGradeint;
	m_ucGradeint = data;
}
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}


	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;
	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;
	if (m_rgbaBitMap) delete[] m_rgbaBitMap;
	if (m_rgbaBrush) delete[] m_rgbaBrush;
	if (m_undoBitMap) delete[] m_undoBitMap;
	m_ucBitmap		= data;
	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);
	m_rgbaBitMap = new unsigned char[width * height * 4];
	memset(m_rgbaBitMap, 0, width * height * 4);
	m_rgbaBrush = new unsigned char[width * height * 4];
	memset(m_rgbaBrush, 0, width * height * 4);
	m_undoBitMap = new unsigned char[width * height * 4];
	memset(m_undoBitMap, 0, width * height * 4);
	if (loadingMuralImage) {
		memcpy(m_rgbaBrush, temp_m_ucPainting, oldPaintHeight * oldPaintWidth * 4);/// PLease take a look for this should store things to m_rgbaBrush but may 
		//need to fix width and height issue can check check 
		loadingMuralImage = false;
	}
	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->state = NORMAL_VIEW;
	m_pUI->m_origView->refresh();
	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	updateBackGroundAlpha();
	m_pUI->m_paintView->resetBackGround();
	m_pUI->m_paintView->resetBrush();
	m_pUI->m_paintView->refresh();
	return 1;
}

int ImpressionistDoc::loadAlphaImage(char* name) {
	// try to open the image to read
	unsigned char* data;
	int				width,
		height;

	if ((data = readBMP(name, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}
	m_nAlphaWidth = width;
	m_nAlphaHeight = height;
	if (m_uAlphaMap) delete[] m_uAlphaMap;
	m_uAlphaMap = data;
	((AlphaBrush*)ImpBrush::c_pBrushes[BRUSH_ALPHAMAP])->updateAlphaImageBrush(m_uAlphaMap, m_nAlphaWidth, m_nAlphaHeight);

	return 1;

}



void ImpressionistDoc::updateBackGroundAlpha() {
	m_pUI->m_paintView->updateBackGroundAlpha(m_pUI->getBackGroundAlpha());
	m_pUI->m_paintView->refresh();
}

void ImpressionistDoc::clearImage(unsigned char*& img) {
	delete[] img;
	img = nullptr;
}
int ImpressionistDoc::loadImagetoBitMap(char* iname, unsigned char*& bitmap, int& mpwidth,int& mpheight) {
	// try to open the image to read
	unsigned char* data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}
	// reflect the fact of loading the new image
	mpwidth = width;
	mpheight = height;
	// release old storage
	if (bitmap) delete[] bitmap;
	bitmap = data;
	return 1;
}

int ImpressionistDoc::blendImage(unsigned char* img1,int width1,int height1, unsigned char* img2, int width2, int height2) {

	// reflect the fact of loading the new image
	m_nWidth = width1+width2-width1/2;
	m_nPaintWidth = width1 + width2 - width1 / 2;
	m_nHeight = max(height1,height2);
	m_nPaintHeight = max(height1, height2);
	// release old storage
	if (m_ucBitmap) delete[] m_ucBitmap;
	if (m_ucPainting) delete[] m_ucPainting;
	if (m_undoBitMap) delete[] m_undoBitMap;
	if (m_rgbaBitMap) delete[] m_rgbaBitMap;
	 m_rgbaBitMap = nullptr;
	if (m_rgbaBrush) delete[] m_rgbaBrush;
	m_rgbaBrush = nullptr;
	int width = m_nWidth;
	int height = m_nHeight;

	m_ucBitmap = new unsigned char[width * height * 3];
	memset(m_ucBitmap, 0, width * height * 3);
	m_ucPainting = new unsigned char[width * height * 3];
	memset(m_ucPainting, 0, width * height * 3);
	m_undoBitMap = new unsigned char[width * height * 3];
	memset(m_undoBitMap, 0, width * height * 3);
	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
		m_pUI->m_mainWindow->y(),
		width * 2,
		height + 25);
	m_pUI->m_origView->resizeWindow(width, height);
	m_pUI->m_origView->state = BLEND_VIEW;
	m_pUI->m_origView->refresh();
	m_pUI->m_paintView->resizeWindow(width, height);
	m_pUI->m_paintView->resetBackGround();
	m_pUI->m_paintView->resetBrush();
	m_pUI->m_paintView->refresh();
	return 1;
}
//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------




int ImpressionistDoc::saveImage(char *iname) 
{

	m_pUI->m_paintView->RestorePreviousDataRGBA(m_pUI->m_paintView->getrgbaBrush(), GL_BACK);
	m_pUI->m_paintView->SaveCurrentContent(GL_BACK);
	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);


	return 1;
}



//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if (m_rgbaBitMap) {
		delete[] m_rgbaBitMap;
		m_rgbaBitMap = new unsigned char[m_nPaintWidth * m_nPaintHeight * 4];
	
		memset(m_rgbaBitMap, 0, m_nPaintWidth * m_nPaintHeight * 4);
		m_pUI->m_paintView->resetBackGround(m_rgbaBitMap);
	}
	if (m_rgbaBrush) {
		delete[] m_rgbaBrush;
		m_rgbaBrush = new unsigned char[m_nPaintWidth * m_nPaintHeight * 4];
		memset(m_rgbaBrush, 0, m_nPaintWidth * m_nPaintHeight * 4);
		m_pUI->m_paintView->resetBrush(m_rgbaBrush);
	}
	if (m_undoBitMap) {
		delete []m_undoBitMap;
		m_undoBitMap = new  unsigned char[m_nPaintWidth * m_nPaintHeight * 4];
		memset(m_undoBitMap, 0, m_nPaintWidth * m_nPaintHeight * 4);
		m_pUI->m_paintView->resetUndo(m_undoBitMap);
	}

	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;
		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);
		// refresh paint view as well	
	}
	m_pUI->m_paintView->refresh();

	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

