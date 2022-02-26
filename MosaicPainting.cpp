#include "MosaicPainting.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <math.h>
#include <algorithm>
#include <string>
#include "impressionistDoc.h"
#include "impressionistUI.h"


void MosaicPainting::loadOriginalImage(char* fname) {
    int width, height;
    if ((originalImage = readBMP(fname, width, height)) == NULL)
    {
        fl_alert("Can't load bitmap file");
        return ;
    }
    originalImageWidth = width;
    originalImageHeight = height;
    outputImage = new unsigned char[width * height * 4];
}

void MosaicPainting::loadImages() {
	for (auto fileName: fileNames) {
        int width, height;
        char tempFileName[200];
        strcpy(tempFileName, fileName.c_str());
        unsigned char* image = readBMP(tempFileName, width, height);
        images.push_back(image);
        fileWidth.push_back(width);
        fileHeight.push_back(height);
	}
}

void MosaicPainting::resizeImage() {


}

vector<float> MosaicPainting::getGridAvgColor(int x, int y, int width, int height) {
    float r = 0, g = 0, b = 0;
    int startPt = y * originalImageWidth + x;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            r += originalImage[(startPt + j + i * originalImageWidth) * 3];
            g += originalImage[(startPt + j + i * originalImageWidth) * 3 + 1];
            b += originalImage[(startPt + j + i * originalImageWidth) * 3 + 2];
        }
    }

    vector<float> partialGridAvgColors;
    partialGridAvgColors.push_back(r / mosaicSize / mosaicSize);
    partialGridAvgColors.push_back(g / mosaicSize / mosaicSize);
    partialGridAvgColors.push_back(b / mosaicSize / mosaicSize);
    return partialGridAvgColors;
}

void MosaicPainting::processRemainGrid(int x, int y, bool last_col, bool last_row) {
    int searchWidth, searchHeight;
    if (last_col && !last_row) {
        searchWidth = remainCol, searchHeight = mosaicSize;
    }
    else if (!last_col && last_row) {
        searchWidth = mosaicSize, searchHeight = remainRow;
    }
    else {
        searchWidth = remainCol, searchHeight = remainRow;
    }
    vector<float> partialGridAvgColors = getGridAvgColor(x, y, remainCol, remainRow);
    int mosaicIndex = findBestMosaicImage(partialGridAvgColors);
    fillMosaicImage(x, y, searchWidth, searchHeight, mosaicIndex);
}

void MosaicPainting::fillMosaicImage(int x, int y, int width, int height, int mosaicIndex) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = GetUI();
    int startPt = y * originalImageWidth + x;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            dlg->m_paintView->rgbaBrush[(startPt + j + i * originalImageWidth) * 4 ] = images[mosaicIndex][(i * mosaicSize + j) * 3];
            dlg->m_paintView->rgbaBrush[(startPt + j + i * originalImageWidth) * 4 + 1] = images[mosaicIndex][(i * mosaicSize + j) * 3 + 1];
            dlg->m_paintView->rgbaBrush[(startPt + j + i * originalImageWidth) * 4 + 2] = images[mosaicIndex][(i * mosaicSize + j) * 3 + 2];
            dlg->m_paintView->rgbaBrush[(startPt + j + i * originalImageWidth) * 4 + 3] = 255;
        }
    }
}

ImpressionistDoc* MosaicPainting::GetDocument(void)
{
    return m_pDoc;
}

ImpressionistUI* MosaicPainting::GetUI(void)
{
    return m_pUI;
}


void MosaicPainting::processPartialGrid(int x, int y) {
    vector<float> partialGridAvgColors = getGridAvgColor(x,y, mosaicSize, mosaicSize);
    int mosaicIndex = findBestMosaicImage(partialGridAvgColors);
    fillMosaicImage(x, y, mosaicSize, mosaicSize, mosaicIndex);
}

int MosaicPainting::findBestMosaicImage(vector<float> inputAvgColors) {
    vector<vector<float>> testAvgColors = imageAvgColors;
    vector<float> distanceVector;
    for (auto avgColor: testAvgColors) {
        distanceVector.push_back(abs(avgColor[0] - inputAvgColors[0]) + abs(avgColor[1] - inputAvgColors[1]) + abs(avgColor[2] - inputAvgColors[2]));
    }

    int minIndex = -1;
    float minValue = 1000000;
    for (int i = 0; i < distanceVector.size(); i++) {
        if (distanceVector[i] < minValue) {
            minValue = distanceVector[i];
            minIndex = i;
        } 
    }
    return minIndex;
}

void MosaicPainting::generateMosaic() {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = GetUI();

    numMosaicCol = ceil(originalImageWidth / mosaicSize);
    numMosaicRow = ceil(originalImageHeight / mosaicSize);
    remainCol = originalImageWidth % mosaicSize;
    remainRow = originalImageHeight %  mosaicSize;

    for (int i = 0; i < numMosaicCol; i++) {
        for (int j = 0; j < numMosaicRow; j++) {
            std::cout << "Now processing grid: " << i << " " << j << std::endl;
            if ((j == numMosaicCol - 1 && remainCol != 0) || (i == numMosaicRow - 1 && remainRow !=0)) {
                processRemainGrid(i * mosaicSize, j * mosaicSize, j == numMosaicCol - 1 && remainCol != 0, i == numMosaicRow - 1 && remainRow != 0);
            }
            else {
                processPartialGrid(i * mosaicSize, j * mosaicSize);
            }
        }
    }
    dlg->m_paintView->RestorePreviousDataRGBA(dlg->m_paintView->rgbaBrush, GL_BACK);


}

void MosaicPainting::getImagesAvgColor() {
    imageAvgColors.clear();
    for (int image = 0; image < images.size(); image++) {
        float r = 0, g = 0, b = 0;
        for (int i = 0; i < fileWidth[image]; i++) {
            for (int j = 0; j < fileHeight[image]; j++) {
                r += images[image][i * fileHeight[image] + fileWidth[image]];
                g += images[image][i * fileHeight[image] + fileWidth[image] + 1];
                b += images[image][i * fileHeight[image] + fileWidth[image] + 2];
            }
        }
        r /= (fileHeight[image] * fileWidth[image]);
        g /= (fileHeight[image] * fileWidth[image]);
        b /= (fileHeight[image] * fileWidth[image]);
        vector<float> avgColor{ r,g,b };
        imageAvgColors.push_back(avgColor);
    }
}

void MosaicPainting::getAllFileNames() {
    fileNames.clear();
    string dir{ imagePoolDir };
    string search_path = dir + "*.bmp";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    //TCHAR NPath[MAX_PATH];
    //GetCurrentDirectory(MAX_PATH, NPath);
    //string curDir(NPath);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string temp{ imagePoolDir };
                temp = temp + std::string{fd.cFileName, strlen(fd.cFileName) };
                //std::string fileString = fd.cFileName;
                fileNames.push_back(temp);
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
}