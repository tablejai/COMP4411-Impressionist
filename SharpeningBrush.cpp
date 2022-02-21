#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "SharpeningBrush.h"
#include "math.h"
#include "time.h"
#include <iostream>

SharpeningBrush::SharpeningBrush(ImpressionistDoc* pDoc, char* name) :
	KernelBrush(pDoc, name)
{
	kernel = { {0.0 , -1.0, 0.0},
			   {-1.0 , 5.0, -1.0},
			   {0.0 , -1.0, 0.0} };
}
