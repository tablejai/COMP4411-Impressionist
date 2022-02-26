#ifndef MosaicPainting_h
#define MosaicPainting_h

#include "ImpressionistUI.h"
#include "ImpressionistDoc.h"
#include "stdlib.h"
#include "Bitmap.h"
#include <string>
#include <vector>

class ImpressionistDoc;
using namespace std;

 class MosaicPainting {

public:
	const char* imagePoolDir = "image_pool/";
	int mosaicSize = 10;
	void resizeImage();
	void loadOriginalImage(char* fname);
	void loadImages();
	void getAllFileNames();
	void getImagesAvgColor();
	void generateMosaic();
	void processPartialGrid(int, int);
	void processRemainGrid(int, int, bool, bool);
	vector<float> getGridAvgColor(int, int, int, int);
	int findBestMosaicImage(vector<float>);
	void fillMosaicImage(int, int, int, int, int);
	ImpressionistDoc* GetDocument(void);
	ImpressionistUI* GetUI(void);


	unsigned char* originalImage;
	unsigned char* outputImage;
	int originalImageWidth;
	int originalImageHeight;
	int numMosaicRow;
	int numMosaicCol;
	int remainRow;
	int remainCol;
	ImpressionistDoc* m_pDoc;
	ImpressionistUI* m_pUI;

	vector<unsigned char*> images;
	vector<vector<float>> imageAvgColors;
	vector<std::string> fileNames;
	vector<int> fileWidth;
	vector<int> fileHeight;
};

#endif