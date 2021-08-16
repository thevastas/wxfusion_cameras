#pragma once
#include "Common.h"
#include "DALProxy640USB.h"
#include "DALProxy640USBDef.h"
//#include "MainWindow.h"


class ThermalCam
{
private:
	char name[128];
	int timeout = 10;
	int* meta = (int*)malloc(600);
	unsigned short* frame = (unsigned short*)malloc(614400);
	void* paData = NULL;
	short* tableoffset = (short*)malloc(614400);
	int i;
public:

	cv::Mat srcImage = cv::Mat::zeros(480, 640, CV_16U);
	cv::UMat srcuImage;
	HANDLE handle = NULL;
	HANDLE Init();
	void Setup(HANDLE handle);
	cv::UMat GetFrame(HANDLE handle);
	void Close(HANDLE handle);
	unsigned char offset = '1';
	unsigned char gain = '0';
	wxPanel* m_parent;
	cv::UMat uimage;
	cv::UMat uimage2;
	cv::UMat uimage3;
	cv::UMat uimage4;// = cv::UMat(480, 640, CV_16U);
	cv::UMat outImage = cv::UMat::zeros(480, 640, CV_8UC3);
	cv::UMat resizedImage = cv::UMat::zeros(1296, 972, CV_8UC3);// = cv::Mat::zeros(972, 1296, CV_8UC3);
};

