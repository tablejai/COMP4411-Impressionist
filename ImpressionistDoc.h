// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"
#include "ImageCursor.h"
#include "vector"

class ImpressionistUI;

typedef struct {
	int x;
	int y;
	GLubyte color[3];
} ColorData;

typedef enum {
	SINGLE_MODE,
	BLEND_MODE
} Mode;
#define Map(A,i,j,W)  *((char*)A+(i+ 3 * (W)*j))

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		loadImagetoBitMap(char* iname, unsigned char*& bitmap, int& mpwidth, int& mpheight);
	int		blendImage(unsigned char* ,int,int,unsigned char* ,int ,int);
	int		saveImage(char *iname);			// called by the UI to save image
	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setStrokeType(int type);
	int		getSize();						// get the UI size
	int		getWidth();
	int		getAngle();
	GLfloat		getAlpha();
	void	setWidth(int width);
	void	setSize(int size);
	void	setAngle(int angle);
	void	setAlpha(float alpha);
	void    meanfilter();
	char*	getImageName();					// get the current image name
	

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	int				m_nWMap1, m_nHMap1;
	int				m_nWMap2, m_nHMap2;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;
	unsigned char* m_undoBitMap;
	unsigned char* m_uctempBitmap1;
	unsigned char* m_uctempBitmap2;



	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	StrokeDir         c_pStrokes;
	ImageCursor*        m_pCursor;
	std::vector<ColorData> reverseData;
	// Size of the brush.
	int m_nSize;							
	ImpressionistUI*	m_pUI;
// Operations
public:
	void clearImage(unsigned char*&);
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  


private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
