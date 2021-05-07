#include "ThermalCam.h"
HANDLE ThermalCam::Init() {
	//MainWindow* comm = (MainWindow*)m_parent->GetParent();
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
		//comm->m_logpanel->m_logtext->AppendText("LWIR] camera was successfully opened\n");
		LOG(INFO) << "[LWIR] Successfully connected to the camera " << name;
	}
	else {
		LOG(ERROR) << Proxy640USB_GetErrorString(Proxy640USB_IsConnectToModule(handle));

		return 0; //TODO error handling
	}

	return handle;
}

void ThermalCam::Setup(HANDLE handle) {
	//MainWindow* comm = (MainWindow*)m_parent->GetParent();
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
	//cv::Mat srcImage;
	Proxy640USB_GetImage(handle, frame, meta, timeout);


	//begin
	//srcImage = cv::Mat::zeros(480, 640, CV_16U);
	//int sizeBuffer = static_cast<int>(srcImage.ByteCount());
	std::memcpy(srcImage.data, frame, 614400);
	//end
	srcImage.copyTo(srcuImage);

	
	//srcImage = cv::Mat(480, 640, CV_16U, frame);
	
	
	if (!srcuImage.empty()){
		cv::cvtColor(srcuImage, uimage, cv::COLOR_GRAY2BGR);
		cv::convertScaleAbs(uimage, uimage2, 1.0 / 256);
		uimage2.convertTo(uimage3, CV_8UC3);
		//cv::resize(uimage3, resizedImage, cv::Size(1296, 972));
	}

	//Sleep(30);
	//return outImage;
	return resizedImage;

}

void ThermalCam::Close(HANDLE handle) {
	//MainWindow* comm = (MainWindow*)m_parent->GetParent();
	Proxy640USB_DisconnectFromModule(handle);
	//comm->m_logpanel->m_logtext->AppendText("[LWIR] camera closed\n");
	LOG(INFO) << "[LWIR] camera closed";
}