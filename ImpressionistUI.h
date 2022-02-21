//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H> // FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/fl_Color_Chooser.h>
#include <string>
#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"
#include <vector>
#include "ImpBrush.h"
using namespace std;

class ImpressionistUI
{
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window *m_mainWindow;
	Fl_Menu_Bar *m_menubar;

	PaintView *m_paintView;
	OriginalView *m_origView;

	// for brush dialog
	Fl_Window *m_brushDialog;
	Fl_Window* m_kernelDialog;

	Fl_Choice *m_BrushTypeChoice;
	Fl_Choice *m_StrokeDirectionChoice;
	vector<Fl_Input*> kernelInputs;
	Fl_Slider *m_BrushSizeSlider;
	Fl_Slider *m_LineWidthSlider;
	Fl_Slider *m_LineAngleSlider;
	Fl_Slider *m_AlphaSlider;
	Fl_Slider* m_BackGroundSlider;
	Fl_Button* m_loadkernel;
	Fl_Button *m_ClearCanvasButton;
	Fl_Button* m_UndoCanvasButton;
	Fl_Button* m_GradientButton;
	Fl_Button* m_LoadButton1;
	Fl_Button* m_LoadButton2;
	Fl_Button* m_LoadEnter;
	// for color dialog
	Fl_Window* m_colorDialog;
	Fl_Window* m_blendDialog;


	Fl_Color_Chooser* m_ColorChooser;


	// Member functions
	void setDocument(ImpressionistDoc *doc);
	ImpressionistDoc *getDocument();

	void show();
	void resize_windows(int w, int h);

	// Interface to get attribute

	int getSize();
	int getWidth();
	int getAngle();
	GLfloat getAlpha();
	GLfloat getBackGroundAlpha();
	void setBackGroundAlpha(float alpha);
	void setWidth(int width);
	void setSize(int size);
	void setAngle(int angle);
	void setAlpha(float alpha);
	void initkernelDialog();
private:
	ImpressionistDoc *m_pDoc; // pointer to document to communicate with the document

	// All attributes here
	int m_nSize;
	int m_nWidth;
	int m_nAngle;
	GLfloat m_nAlpha;
	GLfloat m_nBackGroundAlpha;
	int* kernelValues;

	// Static class members
	static Fl_Menu_Item menuitems[];
	//static Fl_Menu_Item Functionitems[];
	static Fl_Menu_Item brushTypeMenu[NUM_BRUSH_TYPE + 1];
	static Fl_Menu_Item strokeDirectionMenu[NUM_STROKE_TYPE + 1];

	static ImpressionistUI *whoami(Fl_Menu_ *o);

	// All callbacks here.  Callbacks are declared
	// static
	static void cb_load_image(Fl_Menu_ *o, void *v);
	static void cb_load_blend_image(Fl_Menu_* o, void* v);
	static void cb_load_mural_image(Fl_Menu_* o, void* v);
	static void cb_save_image(Fl_Menu_ *o, void *v);
	static void cb_confirm_load(Fl_Widget* o, void* v);
	static void cb_brushes(Fl_Menu_ *o, void *v);
	static void cb_colors(Fl_Menu_* o, void* v);
	static void cb_clear_canvas(Fl_Menu_ *o, void *v);
	static void cb_exit(Fl_Menu_ *o, void *v);
	static void cb_about(Fl_Menu_ *o, void *v);
	static void cb_brushChoice(Fl_Widget *o, void *v);
	static void cb_strokeChoice(Fl_Widget* o, void* v);
	static void cb_clear_canvas_button(Fl_Widget *o, void *v);
	static void cb_load_blend_doc1(Fl_Widget* o, void* v);
	static void cb_load_blend_doc2(Fl_Widget* o, void* v);
	static void cb_undo_canvas_button(Fl_Widget* o, void* v);
	static void cb_sizeSlides(Fl_Widget *o, void *v);
	static void cb_backgroundalphaSlides(Fl_Widget* o, void* v);
	static void cb_widthSlides(Fl_Widget* o, void* v);
	static void cb_alphaSlides(Fl_Widget* o, void* v);
	static void cb_angleSlides(Fl_Widget* o, void* v);
	static void cb_load_alpha_image(Fl_Menu_* , void* );
	static void cb_AutoPaint(Fl_Menu_*, void*);
	static void cb_load_gradient_image(Fl_Menu_* o, void* v);
	static void cb_custom_gradient(Fl_Widget*,void*);
	static void cb_kernel(Fl_Menu_*,void*);
	static void cb_loadkernel(Fl_Widget*, void*);
	static std::string pathToFileName(char *);
	void initBrushDialog(void);
	void initColorDialog(void);
	void initBlendDialog(void);

};

#endif
