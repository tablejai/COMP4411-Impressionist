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
#include "ImageCursor.h"
// Include individual brush headers here.
#include "PointBrush.h"
#include <iostream>
using namespace std;

#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_undoBitMap = NULL;
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
	std::cout << "Stroke direction: "<< c_pStrokes << std::endl;
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
	if (m_undoBitMap) delete[] m_undoBitMap;


	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);
	m_undoBitMap = new unsigned char[width * height * 3];
	memset(m_undoBitMap, 0, width * height * 3);

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
	m_pUI->m_paintView->refresh();


	return 1;
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
	cout << "the width and height:" << mpwidth << "," << mpheight<<"|" << (bitmap== m_uctempBitmap1) << endl;
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
	int width = m_nWidth;
	int height = m_nHeight;

	cout << "allocate w" << width1 << "allocate h" << height1 << endl;
	cout << "allocate w2 " << width2 << "allocate h2 " << height2 << endl;

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
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
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

