#include "ThermalCam.h"
HANDLE ThermalCam::Init() {
	if (Proxy640USB_GetModuleCount(&i) == eProxy640USBSuccess) {
		LOG(INFO) << "THERMAL Number of cameras found: " << i;
	}
	else LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_GetModuleCount(&i));

	if (Proxy640USB_GetModuleName(0, name, 128) == eProxy640USBSuccess) {
		LOG(INFO) << "THERMAL camera name: " << name;
	}
	else LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_GetModuleName(0, name, 128));

	Proxy640USB_ConnectToModule(0, &handle);

	if (Proxy640USB_IsConnectToModule(handle) == eProxy640USBSuccess) {
		LOG(INFO) << "THERMAL Successfully connected to the camera " << name;
	}
	else {
		LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_IsConnectToModule(handle));
		return 0; //TODO error handling
	}

	return handle;
}

void ThermalCam::Setup(HANDLE handle) {
	//if (Proxy640USB_LoadCurrentTableOffset(handle, 1) == eProxy640USBSuccess) {
	//	LOG(INFO) << "THERMAL Successfully applied camera's offset";
	//}
	//else {
	//	LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_LoadCurrentTableOffset(handle, 2));
	//	return;//TODO error handling
	//}

	//if (Proxy640USB_LoadCurrentTableGain(handle, 0) == eProxy640USBSuccess) {
	//	LOG(INFO) << "THERMAL Successfully applied camera's gains";
	//}
	//else {
	//	LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_LoadCurrentTableGain(handle, 2));
	//	return;//TODO error handling
	//}

	if (Proxy640USB_LoadCurrentShutterlessTables(handle) == eProxy640USBSuccess) {
		LOG(INFO) << "THERMAL Successfully applied camera's shutterless mode";
	}
	else {
		LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_LoadCurrentShutterlessTables(handle));
		return;//TODO error handling
	}

	if (Proxy640USB_SetAGCProcessing(handle, 2) == eProxy640USBSuccess) {
		LOG(INFO) << "THERMAL Successfully enabled camera's AGC mode";
	}
	else {
		LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_SetAGCProcessing(handle, 2));
		return;//TODO error handling
	}

	
		if (Proxy640USB_SetShutterLessProcessing(handle, 1) == eProxy640USBSuccess) {
			LOG(INFO) << "THERMAL Successfully enabled camera's AGC mode";
		}
		else {
			LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_SetShutterLessProcessing(handle, 1));
			return;//TODO error handling
		}
}

cv::Mat ThermalCam::GetFrame(HANDLE handle) {
	cv::Mat srcImage;
	Proxy640USB_GetImage(handle, frame, meta, timeout);
	
	srcImage = cv::Mat(480, 640, CV_16U, frame);
	cv::cvtColor(srcImage, srcImage, cv::COLOR_GRAY2BGR);
	cv::convertScaleAbs(srcImage, srcImage, 1.0/256);
	srcImage.convertTo(srcImage, CV_8UC3);

	return srcImage;

}

void ThermalCam::Close(HANDLE handle) {
	Proxy640USB_DisconnectFromModule(handle);
	LOG(INFO) << "THERMAL camera closed";
}