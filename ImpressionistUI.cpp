//
// impressionistUI.h
//
// The user interface part for the program.
//

#include <FL/fl_ask.h>

#include <math.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"
#include <sstream>      // std::stringstream
#include <string>
#include <iostream>

using namespace std;

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the
project.  You can copy and paste these into your code and then change them to
make them look how you want.  Descriptions for all of the widgets here can be found
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------

	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions

		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
	m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v)
	{
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
	mySlider->labelfont(FL_COURIER);
	mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}


//------------Choice---------------------------------------

	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);

	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------

	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

	//---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI *ImpressionistUI::whoami(Fl_Menu_ *o)
{
	return ((ImpressionistUI *)(o->parent()->user_data()));
}

//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_ *o, void *v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();
	char *newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL)
	{
		pDoc->loadImage(newfile);
	}
}
void ImpressionistUI::cb_load_gradient_image(Fl_Menu_* o, void* v) {
	ImpressionistDoc* pDoc = whoami(o)->getDocument();
	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL)
	{
		pDoc->loadGradientImage(newfile);
	}
}

void ImpressionistUI::cb_load_mural_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL)
	{
		int newFileWidth;
		int newFileHeight;
		unsigned char* data;
		if ((data = readBMP(newfile, newFileWidth, newFileHeight)) == NULL) {
			fl_alert("Can't load bitmap file");
			return;
		}
		if (newFileWidth != pDoc->m_nWidth || newFileHeight != pDoc->m_nHeight) {
			fl_alert("Can't load bitmap file with different dimension");
			return;
		}
		pDoc->loadingMuralImage = true;
		pDoc->saveOldImage();
		pDoc->loadImage(newfile);
	}
}


void ImpressionistUI::cb_paintly(Fl_Menu_* o, void* v) {
	ImpressionistUI* pUI = whoami(o);
	pUI->m_paintlyDialog->show();
}
void ImpressionistUI::cb_load_blend_image(Fl_Menu_*o,void*v) {
	ImpressionistDoc* pDoc = whoami(o)->getDocument();
	pDoc->clearImage(pDoc->m_uctempBitmap1);
	pDoc->clearImage(pDoc->m_uctempBitmap2);
	whoami(o)->m_blendDialog->show();

}
void ImpressionistUI::cb_load_alpha_image(Fl_Menu_* o, void* v) {
	ImpressionistDoc* pDoc = whoami(o)->getDocument();
	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL)
	{
		pDoc->loadAlphaImage(newfile);
	}
}
//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_ *o, void *v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char *newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp");
	if (newfile != NULL)
	{
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_ *o, void *v)
{
	whoami(o)->m_brushDialog->show();
}

void ImpressionistUI::cb_colors(Fl_Menu_* o, void* v)
{
	whoami(o)->m_colorDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_ *o, void *v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	pDoc->clearCanvas();
}
void ImpressionistUI::cb_kernel(Fl_Menu_*o, void*v){

	whoami(o)->m_kernelDialog->show();
}

void ImpressionistUI::tokenize(std::string const& str, const char delim,
	std::vector<std::string>& out)
{
	// construct a stream from the string 
	std::stringstream ss(str);

	std::string s;
	while (std::getline(ss, s, delim)) {
		out.push_back(s);
	}
}

void ImpressionistUI::cb_update_kernelSizeInput(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));

	const char* sizeInput = ((Fl_Input*)o)->value();
	strcpy(pUI->m_kernelSizeInputValue, sizeInput);
}

void ImpressionistUI::cb_update_kernelWeightInput(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));

	const char* weightInput = ((Fl_Input*)o)->value();
	strcpy(pUI->m_kernelWeightInputValue, weightInput);
}

void ImpressionistUI::cb_loadkernel(Fl_Widget* o,void* v) {
	//const char* testInput = ((ImpressionistUI*)(o->user_data()))->m_kernelWeightInput->value();
	//std::cout << testInput << std::endl;
	// const char* weightInput_t = strcpy(m_kernelWeightInputValue;
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();


	const char* tempWeightInput = pUI->m_kernelWeightInputValue;
	std::string weightInput(tempWeightInput);
	std::vector<std::string> tokenizedWeight;
	tokenize(weightInput, ' ', tokenizedWeight);

	const char* tempSizeInput = pUI->m_kernelSizeInputValue;
	int sizeInput = atoi(tempSizeInput);
	if (sizeInput <= 0) {
		fl_alert("Input Kernel Size Not Valid ");
		return;
	}
	int numElements = tokenizedWeight.size();
	if (numElements != (sizeInput * sizeInput)) {
		fl_alert("Number of elements does not match kernel size");
		return;
	}
	vector<vector<float>> kernelMatrix;
	for (int i = 0; i < sizeInput; i++) {
		vector<float> kernelRow;
		for (int j = 0; j < sizeInput; j++) {
			float input = std::stof(tokenizedWeight[i* sizeInput + j]);
			kernelRow.push_back(input);
		}
		kernelMatrix.push_back(kernelRow);
	}

	std::cout << pUI->m_kernelNormalizeBox->value() << std::endl;
	bool isNormalize = pUI->m_kernelNormalizeBox->value();

	if (isNormalize) {
		float denom = sizeInput * sizeInput;
		for (int i = 0; i < sizeInput; i++) {
			for (int j = 0; j < sizeInput; j++) {
				kernelMatrix[i][j] /= denom;
			}	
		}
	}

	ImpBrush* originalBrush = pDoc->m_pCurrentBrush;
	pDoc->setBrushType(AUTO_KERNEL_BRUSH);
	int w = pUI->m_paintView->m_nDrawHeight;
	int h = pUI->m_paintView->m_nDrawWidth;

	((KernelBrush*)(pDoc->m_pCurrentBrush))->size = w > h? w : h;
	((KernelBrush*)(pDoc->m_pCurrentBrush))->alpha = 1.0;
	((KernelBrush*)(pDoc->m_pCurrentBrush))->kernel = kernelMatrix;

	Point source(h/2 + pUI->m_paintView->m_nStartCol, pUI->m_paintView->m_nEndRow - w/2);
	Point target(h/2, pUI->m_paintView->m_nWindowHeight - w/2);
	pDoc->m_pCurrentBrush->BrushMove(source, target);

	pUI->m_paintView->RestorePreviousDataRGBA(pUI->m_paintView->rgbaBrush, GL_BACK);
	pDoc->m_pCurrentBrush = originalBrush;
}
//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_ *o, void *v)
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();
}

//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_ *o, void *v)
{
	fl_message("Impressionist FLTK version for CS341, Spring 2002");
}


//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush
// choice.
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget *o, void *v)
{
	ImpressionistUI *pUI = ((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc *pDoc = pUI->getDocument();

	int type = (int)((size_t)v);

	pDoc->setBrushType(type);
	if (type == 1 || type== 4) {
		pUI->m_LineAngleSlider->activate();
		pUI->m_LineWidthSlider->activate();
	}
	else {
		pUI->m_LineAngleSlider->deactivate();
		pUI->m_LineWidthSlider->deactivate();
	}
}
void ImpressionistUI::cb_updatePaintly(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	pUI->m_paintView->paintlymode = (PaintlyMode)((size_t)v);
	if (pUI->m_paintView->paintlymode != Custom) {
		pUI->m_paintView->updatePaintlyPara();
		pUI->m_threshold->value(pUI->m_paintView->threshold);
		pUI->m_threshold->deactivate();
		pUI->m_curvefactor->value(pUI->m_paintView->fc);
		pUI->m_curvefactor->deactivate();
		pUI->m_blurrfactor->value(pUI->m_paintView->blurrSize);
		pUI->m_blurrfactor->deactivate();
		pUI->m_maxStrokeLength->value(pUI->m_paintView->maxStrokeLength);
		pUI->m_maxStrokeLength->deactivate();
		pUI->m_minStrokeLength->value(pUI->m_paintView->minStrokeLength);
		pUI->m_minStrokeLength->deactivate();
		pUI->alpha->value(pUI->m_paintView->a);
		pUI->alpha->deactivate();
	}
	else {
		pUI->m_paintView->updatePaintlyPara();
		pUI->m_threshold->activate();
		pUI->m_curvefactor->activate();
		pUI->m_blurrfactor->activate();
		pUI->m_maxStrokeLength->activate();
		pUI->m_minStrokeLength->activate();
		pUI->alpha->activate();
	}
	cout << pUI->m_paintView->paintlymode << endl;
}

void ImpressionistUI::cb_mosaicPainting(Fl_Menu_* o, void* v) {
	whoami(o)->m_mosaicDialog->show();

}

void ImpressionistUI::cb_load_mosaic_source(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	cout << "open new file:" << newfile << endl;

	if (newfile != "")
	{		
		char* fileName = new char[100];
		strcpy(fileName, pathToFileName(newfile).c_str());
		cout << fileName << endl;
		((Fl_Widget*)o)->label(fileName);
		pUI->mosaicPaintingEngine.loadOriginalImage(fileName);
	}
	else {
		fl_alert("Image name cannot be blank");
	}
	
}

void ImpressionistUI::cb_confirm_mosaic(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();
	
	pUI->mosaicPaintingEngine.generateMosaic();
}


void ImpressionistUI::cb_strokeChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();
	int type = (int)((size_t)v);
	pDoc->setStrokeType(type);
	if (type == STROKE_SLIDER_OR_RIGHT_MOUSE) {
		((ImpressionistUI*)(o->user_data()))->m_nAngle = int(((Fl_Slider*)o)->value());
	}
}
void ImpressionistUI::cb_AutoPaint(Fl_Menu_*o, void*v) {
	ImpressionistUI* pUI = whoami(o);
	pUI->m_paintView->autoPaint();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget *o, void *v)
{
	ImpressionistDoc *pDoc = ((ImpressionistUI *)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}
void ImpressionistUI::cb_undo_canvas_button(Fl_Widget* o, void* v) {
	PaintView* paintView = ((ImpressionistUI*)(o->user_data()))->m_paintView;
	if(paintView !=nullptr)
		paintView->undo();
}
void ImpressionistUI::cb_custom_gradient(Fl_Widget*o,void* v) {
	ImpressionistUI* pUI =((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	if (pUI->m_paintView->getGradientMode() == CUSTOM) {
		pUI->m_paintView->setGradientMode(DEFUALT);
		o->label("DEFAULT MODE");
	}
	else
	{
		if (pDoc->m_ucGradeint != nullptr) {
			pUI->m_paintView->setGradientMode(CUSTOM);
			o->label("CUSTOM GRADIENT MODE");
		}
		else {
			fl_alert("Please load Gradient Image.");
		}
	}

}
string ImpressionistUI::pathToFileName(char* name) {
	int i = 0;
	stringstream ss(name);
	string temp;
	while (getline(ss, temp,'\/')) {
		cout << temp << endl;
	}
	cout << temp << endl;
	return temp ;
}
void ImpressionistUI::cb_load_blend_doc1(Fl_Widget* o, void* v) {
	ImpressionistDoc* pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	cout << "open new file:" << newfile << endl;

	if(newfile!="")
	pDoc->loadImagetoBitMap(newfile, pDoc->m_uctempBitmap1, pDoc->m_nWMap1, pDoc->m_nHMap1);
	;
	char* fileName = new char[100];
	strcpy(fileName, pathToFileName(newfile).c_str());
	cout << fileName << endl;
	((Fl_Widget*)o)->label(fileName);


	
}
void ImpressionistUI::cb_load_blend_doc2(Fl_Widget* o, void* v) {
	ImpressionistDoc* pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
//	((Fl_Widget*)o)->label(newfile);
	cout << "open new file:" << newfile << endl;
	if (newfile != "")
	pDoc->loadImagetoBitMap(newfile, pDoc->m_uctempBitmap2, pDoc->m_nWMap2, pDoc->m_nHMap2);
	char* fileName = new char[100];
	strcpy(fileName, pathToFileName(newfile).c_str());
	cout << fileName << endl;
	((Fl_Widget*)o)->label(fileName);

}
void ImpressionistUI::cb_confirm_load(Fl_Widget* o, void* v) {
	ImpressionistDoc* pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	pDoc->blendImage(pDoc->m_uctempBitmap1, pDoc->m_nWMap1, pDoc->m_nHMap1, pDoc->m_uctempBitmap2, pDoc->m_nWMap2, pDoc->m_nHMap2);
}
//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget *o, void *v)
{
	((ImpressionistUI *)(o->user_data()))->m_nSize = int(((Fl_Slider *)o)->value());
}
void ImpressionistUI::cb_widthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nWidth = int(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAlpha = GLfloat(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_backgroundalphaSlides(Fl_Widget* o, void* v) {
	((ImpressionistUI*)(o->user_data()))->m_nBackGroundAlpha = GLfloat(((Fl_Slider*)o)->value());
	((ImpressionistUI*)(o->user_data()))->m_pDoc->updateBackGroundAlpha();

}

void ImpressionistUI::cb_angleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAngle = int(((Fl_Slider*)o)->value());
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc *ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show()
{
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h)
{
	m_paintView->size(w, h);
	m_origView->size(w, h);
}

//------------------------------------------------
// Set the ImpressionistDoc used by the UI to
// communicate with the brushes
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc *doc)
{
	m_pDoc = doc;
	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}
//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize(int size)
{
	m_nSize = size;

	if (size <= 40)
		m_BrushSizeSlider->value(m_nSize);
}

// Return the brush width
//------------------------------------------------
int ImpressionistUI::getWidth()
{
	return m_nWidth;
}
void ImpressionistUI::setWidth(int width) {
	m_nWidth = width;
	if (width <= 40) {
		m_LineWidthSlider->value(m_nWidth);
	}
}

GLfloat ImpressionistUI::getAlpha() {
	return m_nAlpha;
}
void ImpressionistUI::setAlpha(float alpha) {
	 m_nAlpha = alpha;
	 if (alpha <= 1) {
		 m_AlphaSlider->value(m_nAlpha);

	 }
}
GLfloat ImpressionistUI::getBackGroundAlpha() {
	return m_nBackGroundAlpha;
}
void ImpressionistUI::setBackGroundAlpha(float alpha) {
	m_nBackGroundAlpha = alpha;
	if (alpha <= 1) {
		m_BackGroundSlider->value(m_nBackGroundAlpha);

	}
}
int ImpressionistUI::getAngle() {
	return m_nAngle;
}
void ImpressionistUI::setAngle(int angle) {
	m_nAngle = angle;
	if (angle <= 359) {
		m_LineAngleSlider->value(m_nAngle);
	}
}


// Main menu definition//cb_load_gradient_image
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{"&File", 0, 0, 0, FL_SUBMENU},
	{"&Load Image...", FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image},
	{"&Load Blending Image...", FL_ALT + 'l', (Fl_Callback*)ImpressionistUI::cb_load_blend_image},
	{"&Load Alpha Mapped Image...", FL_ALT + 'l', (Fl_Callback*)ImpressionistUI::cb_load_alpha_image},
	{"&Load Gradient Image...", FL_ALT + 'q', (Fl_Callback*)ImpressionistUI::cb_load_gradient_image},
	{"&Save Image...", FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image},
	{"&Brushes...", FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes},
	{"&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER},
	{"&Apply Kernel...", FL_ALT + 'k', (Fl_Callback*)ImpressionistUI::cb_kernel},
	{"&Colors...", FL_ALT + 'k', (Fl_Callback*)ImpressionistUI::cb_colors},
	{"&Paintly...", FL_ALT + 'p', (Fl_Callback*)ImpressionistUI::cb_paintly},
	//{"&Load Edge Image...", FL_ALT + 'e', (Fl_Callback*)ImpressionistUI::cb_brushes},
	//{"&Load Another Image...", FL_ALT + 'a', (Fl_Callback*)ImpressionistUI::cb_clear_canvas},
	{"&Mosaic...", FL_ALT + 'p', (Fl_Callback*)ImpressionistUI::cb_mosaicPainting},
	{"&Load Mural Image...", FL_ALT + 'j', (Fl_Callback*)ImpressionistUI::cb_load_mural_image},
	{"&Quit", FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit},
	{"&Help", 0, 0, 0, FL_SUBMENU},
	{"&About", FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about},
	{"&Function", 0,0,0,FL_SUBMENU},
	{"&Auto Draw...", FL_ALT + 'a', (Fl_Callback*)ImpressionistUI::cb_AutoPaint},
	{0}};


// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE + 1 - 1] = {
	{"Points", FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
	{"Lines", FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
	{"Circles", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
	{"Scattered Points", FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
	{"Scattered Lines", FL_ALT + 'm', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
	{"Scattered Circles", FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
	{"Star", FL_ALT + 'e', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_STAR},
	{"Traingle", FL_ALT + 'f', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_TRAINGLE},
	{"Alpha Map Brush", FL_ALT + 'g', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_ALPHAMAP},
	{"Blurring", FL_ALT + 'e', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_BLURRRING},
	{"Sharpening", FL_ALT + 'f', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_SHARPENING},
	{"CurveBrush", FL_ALT + 'f', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_CURVE},
	{0}};

Fl_Menu_Item ImpressionistUI::m_paintlyMenu[NumOfPaintly + 1] = {
	{"Impressionist", FL_ALT + 'p', (Fl_Callback*)ImpressionistUI::cb_updatePaintly, (void*)Impressionist},
	{"Expressionist", FL_ALT + 'l', (Fl_Callback*)ImpressionistUI::cb_updatePaintly, (void*)Expressionist},
	{"Pointillist", FL_ALT + 'q', (Fl_Callback*)ImpressionistUI::cb_updatePaintly, (void*)Pointillist},
	{"Psychedelic", FL_ALT + 'a', (Fl_Callback*)ImpressionistUI::cb_updatePaintly, (void*)Psychedelic},
	{"Custom", FL_ALT + 's', (Fl_Callback*)ImpressionistUI::cb_updatePaintly, (void*)Custom},
	{0} };

Fl_Menu_Item ImpressionistUI::strokeDirectionMenu[NUM_STROKE_TYPE + 1] = {
	{"Slider/Right Mouse", FL_ALT + 's', (Fl_Callback*)ImpressionistUI::cb_strokeChoice, (void*)STROKE_SLIDER_OR_RIGHT_MOUSE},
	{"Gradient", FL_ALT + 'g', (Fl_Callback*)ImpressionistUI::cb_strokeChoice, (void*)STROKE_GRADIENT},
	{"Brush Direction", FL_ALT + 'b', (Fl_Callback*)ImpressionistUI::cb_strokeChoice, (void*)STROKE_BRUSH_DIRECTION},
	{0} };

//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
void ImpressionistUI::initBrushDialog() {
	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");

	// Add a brush type choice to the dialog
	m_BrushTypeChoice = new Fl_Choice(50, 10, 150, 25, "&Brush");
	m_BrushTypeChoice->user_data((void*)(this)); // record self to be used by static callback functions
	m_BrushTypeChoice->menu(brushTypeMenu);
	m_BrushTypeChoice->callback(cb_brushChoice);

	m_StrokeDirectionChoice = new Fl_Choice(115, 45, 150, 25, "&Stroke Direction");
	m_StrokeDirectionChoice->user_data((void*)(this)); // record self to be used by static callback functions
	m_StrokeDirectionChoice->menu(strokeDirectionMenu);
	m_StrokeDirectionChoice->callback(cb_strokeChoice);

	m_ClearCanvasButton = new Fl_Button(240, 10, 150, 25, "&Clear Canvas");
	m_ClearCanvasButton->user_data((void*)(this));
	m_ClearCanvasButton->callback(cb_clear_canvas_button);

	m_UndoCanvasButton = new Fl_Button(270, 40, 120, 25, "&Undo Canvas");
	m_UndoCanvasButton->user_data((void*)(this));
	m_UndoCanvasButton->callback(cb_undo_canvas_button);

	m_GradientButton = new Fl_Button(0, 300, 190, 25, "&Use Custom Gradient Image");
	m_GradientButton->user_data((void*)(this));
	m_GradientButton->callback(cb_custom_gradient);
	// init values
	m_nSize = 10;
	m_nWidth = 1;
	m_nAngle = 0;
	m_nAlpha = 1.0;
	m_nBackGroundAlpha = 0.5;

	// Add brush size slider to the dialog
	m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
	m_BrushSizeSlider->user_data((void*)(this)); // record self to be used by static callback functions
	m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
	m_BrushSizeSlider->labelfont(FL_COURIER);
	m_BrushSizeSlider->labelsize(12);
	m_BrushSizeSlider->minimum(1);
	m_BrushSizeSlider->maximum(40);
	m_BrushSizeSlider->step(1);
	m_BrushSizeSlider->value(m_nSize);
	m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
	m_BrushSizeSlider->callback(cb_sizeSlides);

	m_LineWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width");
	m_LineWidthSlider->user_data((void*)(this)); // record self to be used by static callback functions
	m_LineWidthSlider->type(FL_HOR_NICE_SLIDER);
	m_LineWidthSlider->labelfont(FL_COURIER);
	m_LineWidthSlider->labelsize(12);
	m_LineWidthSlider->minimum(1);
	m_LineWidthSlider->maximum(40);
	m_LineWidthSlider->step(1);
	m_LineWidthSlider->value(m_nWidth);
	m_LineWidthSlider->align(FL_ALIGN_RIGHT);
	m_LineWidthSlider->callback(cb_widthSlides);
	m_LineWidthSlider->deactivate();

	m_LineAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle");
	m_LineAngleSlider->user_data((void*)(this)); // record self to be used by static callback functions
	m_LineAngleSlider->type(FL_HOR_NICE_SLIDER);
	m_LineAngleSlider->labelfont(FL_COURIER);
	m_LineAngleSlider->labelsize(12);
	m_LineAngleSlider->minimum(0);
	m_LineAngleSlider->maximum(359);
	m_LineAngleSlider->step(1);
	m_LineAngleSlider->value(m_nAngle);
	m_LineAngleSlider->align(FL_ALIGN_RIGHT);
	m_LineAngleSlider->callback(cb_angleSlides);
	m_LineAngleSlider->deactivate();

	m_AlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha");
	m_AlphaSlider->user_data((void*)(this)); // record self to be used by static callback functions
	m_AlphaSlider->type(FL_HOR_NICE_SLIDER);
	m_AlphaSlider->labelfont(FL_COURIER);
	m_AlphaSlider->labelsize(12);
	m_AlphaSlider->minimum(0.0);
	m_AlphaSlider->maximum(1.0);
	m_AlphaSlider->step(0.01);
	m_AlphaSlider->value(m_nAlpha);
	m_AlphaSlider->align(FL_ALIGN_RIGHT);
	m_AlphaSlider->callback(cb_alphaSlides);

	m_BackGroundSlider = new Fl_Value_Slider(10, 210, 300, 20, "BackGround Alpha");
	m_BackGroundSlider->user_data((void*)(this)); // record self to be used by static callback functions
	m_BackGroundSlider->type(FL_HOR_NICE_SLIDER);
	m_BackGroundSlider->labelfont(FL_COURIER);
	m_BackGroundSlider->labelsize(12);
	m_BackGroundSlider->minimum(0.0);
	m_BackGroundSlider->maximum(1.0);
	m_BackGroundSlider->step(0.01);
	m_BackGroundSlider->value(m_nBackGroundAlpha);
	m_BackGroundSlider->align(FL_ALIGN_RIGHT);
	m_BackGroundSlider->callback(cb_backgroundalphaSlides);

	m_brushDialog->end();
}

void ImpressionistUI::initkernelDialog() {
	m_kernelWeightInputValue = new char[100];
	memset(m_kernelWeightInputValue, 0, 100);
	m_kernelSizeInputValue = new char[100];
	memset(m_kernelSizeInputValue, 0, 100);

	m_kernelDialog = new Fl_Window(500, 300, "Kernel Dialog");

	m_kernelSizeInput = new Fl_Int_Input(200, 0, 50, 20, "Filter Size");
	m_kernelSizeInput->value(m_kernelSizeInputValue);
	m_kernelSizeInput->user_data((void*)(this));
	m_kernelSizeInput->callback(cb_update_kernelSizeInput);

	m_kernelWeightInput = new Fl_Input(200, 100, 150, 20, "Weight (split with white space");
	m_kernelWeightInput->value(m_kernelWeightInputValue);
	m_kernelWeightInput->user_data((void*)(this));
	m_kernelWeightInput->callback(cb_update_kernelWeightInput);

	m_kernelNormalizeBox = new Fl_Check_Button(50, 200, 50, 50, "Normalized");
	m_kernelNormalizeBox->user_data((void*)(this));

	m_loadkernel = new Fl_Button(200,200,100,50,"Apply");
	m_loadkernel->callback(cb_loadkernel);
	m_loadkernel->user_data((void*)(this));

	m_kernelDialog->end();

}

void ImpressionistUI::initColorDialog(void) {
	// color dialog definition
	m_colorDialog = new Fl_Window(380, 300, "Color Selection");
	m_ColorChooser = new Fl_Color_Chooser(0, 0, 200, 225, "Color Blending");
	m_ColorChooser->rgb(1.0, 1.0, 1.0);
	m_colorDialog->end();
}

void ImpressionistUI::initBlendDialog(void) {
	// blend dialog definition
	m_blendDialog = new Fl_Window(380, 300, "Blend Selection");
	m_LoadButton1 = new Fl_Button(0, 0, 200, 50, "&Load Image 1");
	m_LoadButton1->user_data((void*)(this));
	m_LoadButton1->callback(cb_load_blend_doc1);
	m_LoadButton2 = new Fl_Button(0,60,200, 50, "&Load Image 2");
	m_LoadButton2->user_data((void*)(this));
	m_LoadButton2->callback(cb_load_blend_doc2);
	m_LoadEnter = new Fl_Button(0, 120, 100, 50, "&Enter");
	m_LoadEnter->user_data((void*)(this));
	m_LoadEnter->callback(cb_confirm_load);
	m_blendDialog->end();
}

void ImpressionistUI::cb_threshold(Fl_Widget* o, void* v) {
		((ImpressionistUI*)(o->user_data()))->m_paintView->threshold= int(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_blurrSize(Fl_Widget* o, void* v) {
	((ImpressionistUI*)(o->user_data()))->m_paintView->blurrSize = int(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_factorC(Fl_Widget* o, void* v) {
	((ImpressionistUI*)(o->user_data()))->m_paintView->fc = int(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_alpha(Fl_Widget* o, void* v) {
	((ImpressionistUI*)(o->user_data()))->m_paintView->a = int(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_maxStrokeLength(Fl_Widget* o, void* v) {
	((ImpressionistUI*)(o->user_data()))->m_paintView->maxStrokeLength = int(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_minStrokeLength(Fl_Widget* o, void* v) {
	((ImpressionistUI*)(o->user_data()))->m_paintView->minStrokeLength = int(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_doPaintly(Fl_Widget* o, void* v) {
	((ImpressionistUI*)(o->user_data()))->m_paintView->drawPaintly();
}
void ImpressionistUI::initPaintlyDialog(void) {

	int row = 10;
     #define NextRow row+=20
	m_paintlyDialog = new Fl_Window(400,400,"Paintly Window");
	
	m_paintlyTypeChoice = new Fl_Choice(50, NextRow, 150, 25, "&Paintly");
	m_paintlyTypeChoice->user_data((void*)(this)); // record self to be used by static callback functions
	m_paintlyTypeChoice->menu(m_paintlyMenu);
	m_threshold = new Fl_Value_Slider(10, NextRow, 300, 20, "Threshold");
	m_threshold->user_data((void*)(this));
	m_threshold->type(FL_HOR_NICE_SLIDER);
	m_threshold->labelfont(FL_COURIER);
	m_threshold->labelsize(12);
	m_threshold->minimum(0.0);
	m_threshold->maximum(100.0);
	m_threshold->step(0.01);
	m_threshold->value(this->m_paintView->threshold);
	m_threshold->align(FL_ALIGN_RIGHT);
	m_threshold->callback(cb_threshold);
	m_threshold->deactivate();
	m_curvefactor = new Fl_Value_Slider(10, NextRow, 300, 20, "Curve Factor");
	m_curvefactor->user_data((void*)(this));
	m_curvefactor->type(FL_HOR_NICE_SLIDER);
	m_curvefactor->labelfont(FL_COURIER);
	m_curvefactor->labelsize(12);
	m_curvefactor->minimum(0.01);
	m_curvefactor->maximum(1.0);
	m_curvefactor->step(0.01);
	m_curvefactor->value(this->m_paintView->fc);
	m_curvefactor->align(FL_ALIGN_RIGHT);
	m_curvefactor->deactivate();
	m_curvefactor->callback(cb_factorC);
	m_blurrfactor = new Fl_Value_Slider(10, NextRow, 300, 20, "Blurr Factor");
	m_blurrfactor->user_data((void*)(this));
	m_blurrfactor->type(FL_HOR_NICE_SLIDER);
	m_blurrfactor->labelfont(FL_COURIER);
	m_blurrfactor->labelsize(12);
	m_blurrfactor->minimum(1.0);
	m_blurrfactor->maximum(20.0);
	m_blurrfactor->step(0.01);
	m_blurrfactor->value(this->m_paintView->blurrSize);
	m_blurrfactor->align(FL_ALIGN_RIGHT);
	m_blurrfactor->deactivate();
	m_blurrfactor->callback(cb_blurrSize);
	m_maxStrokeLength = new Fl_Value_Slider(10, NextRow,300,20,"Max Stroke Length");
	m_maxStrokeLength->user_data((void*)(this));
	m_maxStrokeLength->type(FL_HOR_NICE_SLIDER);
	m_maxStrokeLength->labelfont(FL_COURIER);
	m_maxStrokeLength->labelsize(12);
	m_maxStrokeLength->minimum(5);
	m_maxStrokeLength->maximum(16.0);
	m_maxStrokeLength->step(0.01);
	m_maxStrokeLength->value(this->m_paintView->maxStrokeLength);
	m_maxStrokeLength->callback(cb_maxStrokeLength);
	m_maxStrokeLength->align(FL_ALIGN_RIGHT);
	m_maxStrokeLength->deactivate();
	m_minStrokeLength = new Fl_Value_Slider(10, NextRow, 300, 20, "Min Stroke Length");
	m_minStrokeLength->user_data((void*)(this));
	m_minStrokeLength->type(FL_HOR_NICE_SLIDER);
	m_minStrokeLength->labelfont(FL_COURIER);
	m_minStrokeLength->labelsize(12);
	m_minStrokeLength->minimum(0);
	m_minStrokeLength->maximum(4);
	m_minStrokeLength->step(0.01);
	m_minStrokeLength->value(this->m_paintView->minStrokeLength);
	m_minStrokeLength->align(FL_ALIGN_RIGHT);
	m_minStrokeLength->callback(ImpressionistUI::cb_minStrokeLength);
	m_minStrokeLength->deactivate();
	alpha = new Fl_Value_Slider(10, NextRow, 300, 20, "Alpha");
	alpha->user_data((void*)(this));
	alpha->type(FL_HOR_NICE_SLIDER);
	alpha->labelfont(FL_COURIER);
	alpha->minimum(0.0);
	alpha->maximum(1.0);
	alpha->step(0.01);
	alpha->value(this->m_paintView->a);
	alpha->align(FL_ALIGN_RIGHT);
	alpha->deactivate();
	paintlyButton = new Fl_Button(10, NextRow, 300, 20, "Load Paintly");
	paintlyButton->user_data((void*)(this));
	paintlyButton->callback(cb_doPaintly);
	m_paintlyDialog->end();
	

}
void ImpressionistUI::initMosaicDialog(void) {
	mosaicPaintingEngine = new MosaicPainting();

	mosaicPaintingEngine->getAllFileNames();
	mosaicPaintingEngine->loadImages();
	m_mosaicDialog = new Fl_Window(380, 300, "Mosaic Painting");
	m_loadMosaicSourceButton = new Fl_Button(0, 0, 200, 50, "&Load Source Image");
	m_loadMosaicSourceButton->user_data((void*)(this));
	m_loadMosaicSourceButton->callback(cb_load_mosaic_source);
	m_mosaicEnter = new Fl_Button(0, 120, 100, 50, "&Enter");
	m_mosaicEnter->user_data((void*)(this));
	m_mosaicEnter->callback(cb_confirm_mosaic);
	m_mosaicDialog->end();
}

ImpressionistUI::ImpressionistUI()
{
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
	m_mainWindow->user_data((void *)(this)); // record self to be used by static callback functions
	// install menu bar
	m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
	m_menubar->menu(menuitems);
	//m_menubar->menu(Functionitems);


	// Create a group that will hold two sub windows inside the main
	// window
	Fl_Group *group = new Fl_Group(0, 25, 600, 275);

	// install paint view window
	m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view"); // 0jon
	m_paintView->box(FL_DOWN_FRAME);

	// install original view window
	m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view"); // 300jon
	m_origView->box(FL_DOWN_FRAME);
	m_origView->deactivate();

	group->end();
	Fl_Group::current()->resizable(group);
	m_mainWindow->end();

	initBrushDialog();
	initColorDialog();
	initBlendDialog();
	initkernelDialog();
	initPaintlyDialog();
	initMosaicDialog();
}
