#pragma once
#include "Common.h"
#include "DALProxy384LUSB.h"
#include "DALProxy384LUSBDef.h"
//#include "MainWindow.h"


class ThermalCam384
{
private:
	char name[128];
	int timeout = 10;
	int* meta = (int*)malloc(600);
	unsigned short* frame = (unsigned short*)malloc(200448);
	void* paData = NULL;
	short* tableoffset = (short*)malloc(200448);
	int i;
public:

	cv::Mat m_srcImage = cv::Mat::zeros(288, 384, CV_16U);
	cv::UMat m_srcuImage;
	HANDLE handle = NULL;
	HANDLE Init();
	void Setup(HANDLE handle);
	cv::UMat GetFrame(HANDLE handle);
	void Close(HANDLE handle);
	unsigned char offset = '1';
	unsigned char gain = '0';
	wxPanel* m_parent;
	cv::UMat m_uimage;
	cv::UMat m_uimage2;
	cv::UMat m_uimage3;
	cv::UMat m_uimage4;
	cv::UMat outImage = cv::UMat::zeros(288, 384, CV_8UC3);
	cv::UMat resizedImage = cv::UMat::zeros(1296, 972, CV_8UC3);
};

