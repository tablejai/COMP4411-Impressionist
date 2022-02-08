#include <stdlib.h>
#ifndef  IMG_CURSOR
#define IMG_CURSOR
class ImpressionistDoc; 
class ImageCursor
{
private:
	int x;
	int y;
public:

	
	ImageCursor(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	ImpressionistDoc* GetDocument(void);
	char* CursorName(void);
	void setx(int x);
	void sety(int y);
	void setpos(int x, int y);
	void drawcursor();
	int getx();
	int gety();
private:
	ImpressionistDoc* m_pDoc;
	char*m_nCursorName; 
};

#endif