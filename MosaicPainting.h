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
	char* imagePoolDir = "mosaic_image_pool/";
	int mosaicSize = 30;
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


	unsigned char* originalImage;
	unsigned char* outputImage;
	int originalImageWidth;
	int originalImageHeight;
	int numMosaicRow;
	int numMosaicCol;
	int remainRow;
	int remainCol;

	vector<unsigned char*> images;
	vector<vector<float>> imageAvgColors;
	vector<char *> fileNames;
	vector<int> fileWidth;
	vector<int> fileHeight;
};

#endif