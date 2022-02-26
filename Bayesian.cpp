#include "Bayesian.h"
#include <iostream>
#include <cmath>

using namespace std;

#define M_PI 3.14159265358979323846

vector<vector<float>> Bayesian::generateGaussianKernel(float sigma, float size) {
    float mean = size / 2;
    float sum = 0.0; 
    float maxValue = -10;

    vector<vector<float>> kernelMatrix;
    for (int x = 0; x < size; ++x) {
        vector<float> kernelRow;
        for (int y = 0; y < size; ++y) {
            float kernelValue = exp(-0.5 * (pow((x - mean) / sigma, 2.0) + pow((y - mean) / sigma, 2.0)))
                / (2 * M_PI * sigma * sigma);
            kernelRow.push_back(kernelValue);
            maxValue = maxValue > kernelValue ? maxValue : kernelValue;
            sum += kernelValue;
        }
        kernelMatrix.push_back(kernelRow);
    }
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            kernelMatrix[x][y] /= sum;
            kernelMatrix[x][y] /= maxValue;
        }
    }
    
    return kernelMatrix;
}

vector<vector<float>> Bayesian::generateSizedMatrix(float w, float h, float value) {
    vector<vector<float>> matrix;
    for (int i = 0; i < h; i++) {
        vector<float> row;
        for (int j = 0; j < w; j++) {
            row.push_back(value);
        }
        matrix.push_back(row);
    }
    return matrix;
}

vector<vector<float>> Bayesian::getNeighbor() {
    vector<vector<float>> neighbor;
    return neighbor;
}

ImpressionistDoc* Bayesian::GetDocument(void)
{
    return m_pDoc;
}

ImpressionistUI* Bayesian::GetUI(void)
{
    return m_pUI;
}

void Bayesian::matting() {
    foregroundMask = new bool[imageHeight * imageWidth];
    backgroundMask = new bool[imageHeight * imageWidth];
    unknownMask = new bool[imageHeight * imageWidth];
    alphaImage = new unsigned char[imageHeight * imageWidth];

    gaussianKernel = generateGaussianKernel(gaussianSigma, gaussianSize);
    generateMasks();


}

void Bayesian::orchardAndBouman(void) {

}

void Bayesian::generateMasks(void) {
    for (int i = 0; i < imageHeight; i++) {
        for (int j = 0; j < imageWidth; j++) {
            int tar = (i * imageHeight + j) * 3;
            backgroundMask[(tar / 3)] = false;
            foregroundMask[(tar / 3)] = false;
            unknownMask[(tar / 3)] = false;
            if (trimapImage[tar] == 0 && trimapImage[tar + 1] == 0 && trimapImage[tar + 2] == 0) {
                backgroundMask[(tar / 3)] = true;
            }
            else if (trimapImage[tar] == 255 && trimapImage[tar + 1] == 255 && trimapImage[tar + 2] == 255){
                foregroundMask[(tar / 3)] = true;
                alphaImage[(tar / 3)] = 1;
            }
            else {
                unknownMask[(tar / 3)] = true;
            }
        }
    }
}