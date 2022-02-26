#pragma once
#include "impressionistUI.h"
#include "impressionistDoc.h"
#include <vector>
using namespace std;

class ImpressionistUI;
class ImpressionistDoc;

class Bayesian {
public:
	unsigned char* originalImage;
	unsigned char* alphaImage;
	unsigned char* trimapImage;

	bool* foregroundMask;
	bool* backgroundMask;
	bool* unknownMask;

	vector<vector<float>> gaussianKernel;
	vector<vector<vector<float>>> F;
	vector<vector<vector<float>>> B;

	void orchardAndBouman(void);
	vector<vector<float>> getNeighbor(void);
	void matting(void);
	void generateMasks(void);
	vector<vector<float>> generateGaussianKernel(float , float );
	vector<vector<float>> generateSizedMatrix(float, float, float);

	ImpressionistDoc* GetDocument(void);
	ImpressionistUI* GetUI(void);

	
	float sigma = 8;
	float size = 25;
	int gaussianSize = 3;
	float gaussianSigma = 0.5;
	
	int imageWidth;
	int imageHeight;

	ImpressionistUI* m_pUI;
	ImpressionistDoc* m_pDoc;



	
};