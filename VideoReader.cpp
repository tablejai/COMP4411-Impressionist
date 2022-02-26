//#include "VideoReader.h"
//#include <windows.h>
//#include  <vfw.h>
//#include <iostream>
//#include <fstream>
//#pragma comment(lib, "Vfw32.lib")
//using namespace std;
//
///* ????????????? ?????? ?? ???? ????? ? ????.
//???? ????? i - ?? ??????? ????? ???? i mod N = 0,
//????? ??? ???? ?? ???????
//*/
//PAVIFILE pfile;
//void reverse(char* name1, char* name2, char* name3) {
//	PAVIFILE file, file2, file3;
//	PAVISTREAM stream, stream2, stream3;
//	AVISTREAMINFO aviInfo, aviInfo2, aviInfo3;
//	BITMAPINFOHEADER bmpInfo;
//	long bmpInfoSize = sizeof(bmpInfo);
//	void* buffer;
//
//	AVIFileOpen(&file, name1, OF_READ, NULL);
//	AVIFileOpen(&file2, name2, OF_READ, NULL);
//	AVIFileOpen(&file3, name3, OF_CREATE | OF_WRITE, NULL);
//
//	AVIFileGetStream(file, &stream, streamtypeVIDEO, 0);
//	AVIFileGetStream(file2, &stream2, streamtypeVIDEO, 0);
//
//	AVIStreamInfo(stream, &aviInfo, sizeof(aviInfo));
//	AVIStreamInfo(stream2, &aviInfo2, sizeof(aviInfo));
//
//	aviInfo3 = aviInfo;
//	aviInfo3.dwLength = aviInfo.dwLength + aviInfo3.dwLength;
//	AVIFileCreateStream(file3, &stream3, &aviInfo3);
//
//	AVIStreamReadFormat(stream, 0, &bmpInfo, &bmpInfoSize);
//	AVIStreamSetFormat(stream3, 0, &bmpInfo, sizeof(bmpInfo));
//
//	buffer = malloc(bmpInfo.biSizeImage);
//	int k = 0, i;
//	for (i = aviInfo.dwLength - 1, k = 0; i >= 0; i--) {
//		AVIStreamRead(stream, i, 1, buffer, bmpInfo.biSizeImage, NULL, NULL);
//		AVIStreamWrite(stream3, k, 1, buffer, bmpInfo.biSizeImage, AVIIF_KEYFRAME, NULL, NULL);
//		k++;
//	}
//	for (int j = aviInfo2.dwLength - 1; j >= 0; j--, k++) {
//		AVIStreamRead(stream2, j, 1, buffer, bmpInfo.biSizeImage, NULL, NULL);
//		AVIStreamWrite(stream3, k, 1, buffer, bmpInfo.biSizeImage, AVIIF_KEYFRAME, NULL, NULL);
//	}
//	free(buffer);
//
//	AVIStreamRelease(stream);
//	AVIStreamRelease(stream2);
//	AVIStreamRelease(stream3);
//	AVIFileRelease(file);
//	AVIFileRelease(file2);
//	AVIFileRelease(file3);
//}
//
//void correlation(char* video, char* outFile) {
//	PAVIFILE file;
//	PAVISTREAM stream;
//	AVISTREAMINFO aviInfo;
//	BITMAPINFOHEADER bmpInfo;
//	long bmpInfoSize = sizeof(bmpInfo);
//	RGBTRIPLE* buffer, * buffer2;
//
//	AVIFileOpen(&file, video, OF_READ, NULL);
//	AVIFileGetStream(file, &stream, streamtypeVIDEO, 0);
//	AVIStreamInfo(stream, &aviInfo, sizeof(aviInfo));
//	AVIStreamReadFormat(stream, 0, &bmpInfo, &bmpInfoSize);
//	//	cout << "h = " << bmpInfo.biHeight << endl;
//	//	cout << "w = " << bmpInfo.biWidth << endl;
//	//	cout << "size image = " << bmpInfo.biSizeImage << endl;
//	double size = (double)bmpInfo.biSizeImage;// bmpInfo.biHeight*bmpInfo.biWidth;
//
//	buffer = new RGBTRIPLE[size];
//	buffer2 = new RGBTRIPLE[size];
//	ofstream out(outFile);
//	for (int i = 0; i < aviInfo.dwLength - 1; i++) {
//		long b;
//		AVIStreamRead(stream, i, 1, buffer, size, &b, NULL);
//		AVIStreamRead(stream, i + 1, 1, buffer2, size, &b, NULL);
//		//???.????????
//		double matA = 0, matB = 0, mat = 0, corr = 0;
//
//		for (int j = 0; j < size; j++) {
//			matA += (double)buffer[j].rgbtGreen;
//			matB += (double)buffer2[j].rgbtGreen;
//		}
//		matA = matA / size;
//		matB = matB / size;
//		//????????.??????????
//		double sigma = 0, sigma2 = 0;
//		for (int j = 0; j < size; j++) {
//			sigma += ((double)buffer[j].rgbtGreen - matA) * ((double)buffer[j].rgbtGreen - matA);
//			sigma2 += ((double)buffer2[j].rgbtGreen - matB) * ((double)buffer2[j].rgbtGreen - matB);
//		}
//		sigma = sqrt(sigma / (size - 1.0));
//		sigma2 = sqrt(sigma2 / (size - 1.0));
//
//		for (int j = 0; j < size; j++) {
//			mat += ((double)buffer[j].rgbtGreen - matA) * ((double)buffer2[j].rgbtGreen - matB);
//		}
//		mat = mat / size;
//		corr = mat / (sigma * sigma2);
//		out << i << ";" << corr << endl;
//	}
//	out.close();
//	delete(buffer);
//	delete(buffer2);
//}
//VOID LoadAVIFile(LPCSTR szFile, HWND hwnd)
//{
//	LONG hr;
//	PAVIFILE pfile;
//
//	AVIFileInit();          // opens AVIFile library 
//
//	hr = AVIFileOpen(&pfile, szFile, OF_SHARE_DENY_WRITE, 0L);
//	if (hr != 0) {
//		// Handle failure.
//		return;
//	}
//
//	// 
//	// Place functions here that interact with the open file. 
//	// 
//
//	AVIFileRelease(pfile);  // closes the file 
//	AVIFileExit();          // releases AVIFile library 
//}
//#define MAXNUMSTREAMS 100
//PAVISTREAM* gapavi = new PAVISTREAM[MAXNUMSTREAMS];
//int gcpavi = 0;
//
//
//void InsertAVIFile(PAVIFILE pfile, HWND hwnd, LPSTR lpszFile)
//{
//	int    i;
//	// Open the streams until a stream is not available. 
//	for (i = gcpavi; i < MAXNUMSTREAMS; i++) {
//		gapavi[i] = NULL;
//		if (AVIFileGetStream(pfile, &gapavi[i], 0L, i - gcpavi)
//			!= AVIERR_OK)
//			break;
//
//		if (gapavi[i] == NULL)
//			break;
//	}
//	// Display error message-stream not found. 
//	if (gcpavi == i)
//	{
//		// Handle failure.
//		if (pfile) // If file is open, close it 
//			AVIFileRelease(pfile);
//		return;
//	}
//	else {
//		gcpavi = i - 1;
//	}
//
//	//  . 
//	//  . Place functions to process data here. 
//	//  . 
//}
//void StreamTypes(HWND hwnd)
//{
//	AVISTREAMINFO     avis;
//	LONG    r, lHeight = 0;
//	WORD    w;
//	int     i;
//	RECT    rc;
//
//	// Walk through all streams. 
//	for (i = 0; i < gcpavi; i++) {
//		AVIStreamInfo(gapavi[i], &avis, sizeof(avis));
//
//		if (avis.fccType == streamtypeVIDEO) {
//
//			// Place video-processing functions here. 
//
//		}
//		else if (avis.fccType == streamtypeAUDIO) {
//
//			// Place audio-processing functions here. 
//
//		}
//		else if (avis.fccType == streamtypeTEXT) {
//
//			// Place text-processing functions here. 
//
//		}
//	}
//}
// unsigned char* readVideo(char* fname, int& width, int& height) {
//	AVIFileInit();
//	//module(argv[1], argv[2], "out.avi");
//	//reverse(argv[2], argv[1], "reverse.avi");
//	//correlation(argv[1], "video3.csv");
//	HWND hwnd;
//	InsertAVIFile(fname,hwnd,);
//
//	AVIFileExit();
//	return nullptr;
//}
