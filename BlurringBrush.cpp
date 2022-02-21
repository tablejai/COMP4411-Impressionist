#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "BlurringBrush.h"
#include "math.h"
#include "time.h"
#include <iostream>

BlurringBrush::BlurringBrush(ImpressionistDoc* pDoc, char* name) :
	KernelBrush(pDoc, name)
{
	kernel = { {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}, 
			   {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},  
			   {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0} };
}
 