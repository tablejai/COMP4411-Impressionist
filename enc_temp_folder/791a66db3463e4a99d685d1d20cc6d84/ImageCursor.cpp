#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImageCursor.h"
#include <iostream>
using namespace std;
int x, y;
float i, j;

ImageCursor::ImageCursor(ImpressionistDoc* pDoc, char* name ) : m_pDoc(pDoc),m_nCursorName(name)
{
}

ImpressionistDoc* ImageCursor::GetDocument(void) {
	return m_pDoc;

}
char* ImageCursor::CursorName(void) {
	return 	m_nCursorName;

}
void ImageCursor::setx(int x) {
	this->x = x;
}
void ImageCursor::sety(int y) {
	this->y = y;
}
void ImageCursor::drawcursor() {
	glPointSize((float)3);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2d(x, m_pDoc->m_pUI->m_origView->getHeight() -y);
	
//	cout << m_pDoc->m_pCursor->getx() << "," << m_pDoc->m_pCursor->gety() << endl;
	glEnd();
}
int ImageCursor::getx() {
	return x;
}
int ImageCursor::gety() {
	return y;
}
void ImageCursor::setpos(int x, int y) {
	this->x = x;
	this->y = y;
}