//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;


ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	double r_mult, g_mult, b_mult;
	Fl_Color_Chooser* colorChooser = dlg->m_ColorChooser;

	r_mult = colorChooser->r();
	g_mult = colorChooser->g();
	b_mult = colorChooser->b();

	GLubyte color[3];
	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 ); 
	glColor4f(color[0] / 255.0f * r_mult, color[1] / 255.0f * g_mult, color[2] / 255.0f * b_mult, alpha);
	//glColor3ubv( color );
}


void ImpBrush::initReverseData(){
	ImpressionistDoc* pDoc = GetDocument();
	pDoc->reverseData.clear();
}