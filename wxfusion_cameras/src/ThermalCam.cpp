#include "ThermalCam.h"
HANDLE ThermalCam::Init() {
	if (Proxy640USB_GetModuleCount(&i) == eProxy640USBSuccess) {
		LOG(INFO) << "[LWIR] Number of cameras found: " << i;
	}
	else LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_GetModuleCount(&i));

	if (Proxy640USB_GetModuleName(0, name, 128) == eProxy640USBSuccess) {
		LOG(INFO) << "[LWIR] camera name: " << name;
	}
	else LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_GetModuleName(0, name, 128));

	Proxy640USB_ConnectToModule(0, &handle);

	if (Proxy640USB_IsConnectToModule(handle) == eProxy640USBSuccess) {
		LOG(INFO) << "[LWIR] Successfully connected to the camera " << name;
	}
	else {
		LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_IsConnectToModule(handle));

		return 0; //TODO error handling
	}
	return handle;
}

void ThermalCam::Setup(HANDLE handle) {
	if (Proxy640USB_LoadCurrentShutterlessTables(handle) == eProxy640USBSuccess) {
		LOG(INFO) << "[LWIR] Successfully loaded shutterless tables";
	}
	else {
		LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_LoadCurrentShutterlessTables(handle));
		return;//TODO error handling
	}

	if (Proxy640USB_SetAGCProcessing(handle, 2) == eProxy640USBSuccess) {
		LOG(INFO) << "[LWIR] Successfully enabled camera's AGC mode";
		//comm->m_logpanel->m_logtext->AppendText("[LWIR] automatic gain mode enabled\n");
	}
	else {
		LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_SetAGCProcessing(handle, 2));
		return;//TODO error handling
	}

	
	if (Proxy640USB_SetShutterLessProcessing(handle, 1) == eProxy640USBSuccess) {
		LOG(INFO) << "[LWIR] shutterless mode enabled\n";
	}
	else {
		LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_SetShutterLessProcessing(handle, 1));
		return;//TODO error handling
	}
}

cv::UMat ThermalCam::GetFrame(HANDLE handle) {
	Proxy640USB_GetImage(handle, frame, meta, timeout);
	std::memcpy(srcImage.data, frame, 614400);
	srcImage.copyTo(srcuImage);
	
	
	if (!srcuImage.empty()){
		cv::cvtColor(srcuImage, uimage, cv::COLOR_GRAY2BGR);
		cv::convertScaleAbs(uimage, uimage2, 1.0 / 256);
		cv::resize(uimage2, uimage3, cv::Size(1296, 972));
		uimage3.convertTo(uimage4, CV_8UC3);
		
	}
	return uimage4;

}

void ThermalCam::Close(HANDLE handle) {
	Proxy640USB_DisconnectFromModule(handle);
	LOG(INFO) << "[LWIR] camera closed";
}