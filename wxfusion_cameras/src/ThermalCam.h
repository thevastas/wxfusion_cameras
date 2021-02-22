#pragma once
#include "Common.h"
#include "DALProxy640USB.h"
#include "DALProxy640USBDef.h"
//#include "MainWindow.h"


class ThermalCam
{
private:
	char name[128];
	int timeout = 1;
	int* meta = (int*)malloc(600);
	unsigned short* frame = (unsigned short*)malloc(614400);
	void* paData = NULL;
	short* tableoffset = (short*)malloc(614400);
	int i;
public:


	HANDLE handle = NULL;
	HANDLE Init();
	void Setup(HANDLE handle);
	cv::Mat GetFrame(HANDLE handle);
	void Close(HANDLE handle);
	unsigned char offset = '1';
	unsigned char gain = '0';
	wxPanel* m_parent;
};

