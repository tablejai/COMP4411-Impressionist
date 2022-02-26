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
#define Map4(A,i,j,W)  *((char*)A+(i+ 4 * (W)*j))

void RGB_TO_RGBA(GLvoid* data, unsigned char* RGBA, int w, int h, int a);
void RGBA_TO_RGB(GLvoid* data, unsigned char* RGB, int w, int h, int a);
class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *name);			// called by the UI to load image
	int     loadVideo(char* name);
	int     loadAlphaImage(char* name);
	int     loadEdgeImage(char* name);
	int		loadImagetoBitMap(char* iname, unsigned char*& bitmap, int& mpwidth, int& mpheight);
	void	saveOldImage(void);
	int		blendImage(unsigned char* ,int,int,unsigned char* ,int ,int);
	int		saveImage(char *iname);			// called by the UI to save image
	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setStrokeType(int type);
	int    loadGradientImage(char* name);
	int		getSize();						// get the UI size
	int		getWidth();
	int		getAngle();
	void   updateBackGroundAlpha();
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
	int				m_edgeThreshold = 100;
	int oldPaintWidth, oldPaintHeight;
	int				m_nWMap1, m_nHMap1;
	int				m_nWMap2, m_nHMap2;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	int				m_nAlphaWidth;
	int				m_nAlphaHeight;
	int				m_nGradientWidth;
	int				m_nGradientHeight;
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char* m_edgeView = nullptr;
	unsigned char*	m_ucPainting;
	unsigned char* m_ucGradeint;
	unsigned char* m_rgbaBitMap;
	unsigned char* m_rgbaBrush;
	unsigned char* m_undoBitMap;
	unsigned char* m_uctempBitmap1;
	unsigned char* m_uctempBitmap2;
	unsigned char* m_uAlphaMap;
	int* m_edgeBinView = nullptr;

	unsigned char* temp_m_ucPainting = nullptr;
	bool loadingMuralImage = false;



	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	StrokeDir         c_pStrokes = STROKE_SLIDER_OR_RIGHT_MOUSE;
	ImageCursor*        m_pCursor;
	std::vector<ColorData> reverseData;
	// Size of the brush.
	int m_nSize;							
	ImpressionistUI*	m_pUI;
// Operations
public:
	void ImpressionistDoc::transformEdgeToBinary(void);
	void clearImage(unsigned char*&);
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  
	bool isEdge(const Point);


private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
