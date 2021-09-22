#include "ThermalCam384.h"
HANDLE ThermalCam384::Init() {
	if (Proxy384LUSB_GetModuleCount(&i) == eProxy384LUSBSuccess) {
		LOG(INFO) << "[LWIR384] Number of cameras found: " << i;
	}
	else LOG(ERROR) << Proxy384LUSB_GetErrorString(Proxy384LUSB_GetModuleCount(&i));

	if (Proxy384LUSB_GetModuleName(0, name, 128) == eProxy384LUSBSuccess) {
		LOG(INFO) << "[LWIR384] camera name: " << name;
	}
	else LOG(ERROR) << Proxy384LUSB_GetErrorString(Proxy384LUSB_GetModuleName(0, name, 128));

	Proxy384LUSB_ConnectToModule(0, &handle);

	if (Proxy384LUSB_IsConnectToModule(handle) == eProxy384LUSBSuccess) {
		LOG(INFO) << "[LWIR384] Successfully connected to the camera " << name;
	}
	else {
		LOG(ERROR) << Proxy384LUSB_GetErrorString(Proxy384LUSB_IsConnectToModule(handle));

		return 0; //TODO error handling
	}
	return handle;
}

void ThermalCam384::Setup(HANDLE handle) {
	if (Proxy384LUSB_LoadCurrentShutterlessTables(handle) == eProxy384LUSBSuccess) {
		LOG(INFO) << "[LWIR384] Successfully loaded shutterless tables";
	}
	else {
		LOG(ERROR) << Proxy384LUSB_GetErrorString(Proxy384LUSB_LoadCurrentShutterlessTables(handle));
		return;//TODO error handling
	}

	if (Proxy384LUSB_SetAGCProcessing(handle, 2) == eProxy384LUSBSuccess) {
		LOG(INFO) << "[LWIR384] Successfully enabled camera's AGC mode";
		//comm->m_logpanel->m_logtext->AppendText("[LWIR] automatic gain mode enabled\n");
	}
	else {
		LOG(ERROR) << Proxy384LUSB_GetErrorString(Proxy384LUSB_SetAGCProcessing(handle, 2));
		return;//TODO error handling
	}


	if (Proxy384LUSB_SetShutterLessProcessing(handle, 1) == eProxy384LUSBSuccess) {
		LOG(INFO) << "[LWIR384] shutterless mode enabled\n";
	}
	else {
		LOG(ERROR) << Proxy384LUSB_GetErrorString(Proxy384LUSB_SetShutterLessProcessing(handle, 1));
		return;//TODO error handling
	}
}

cv::UMat ThermalCam384::GetFrame(HANDLE handle) {
	Proxy384LUSB_GetImage(handle, frame, meta, timeout);
	std::memcpy(srcImage.data, frame, 200448);
	srcImage.copyTo(srcuImage);


	if (!srcuImage.empty()) {
		cv::cvtColor(srcuImage, uimage, cv::COLOR_GRAY2BGR);
		cv::convertScaleAbs(uimage, uimage2, 1.0 / 256);
		cv::resize(uimage2, uimage3, cv::Size(1296, 972));
		uimage3.convertTo(uimage4, CV_8UC3);

	}
	return uimage4;

}

void ThermalCam384::Close(HANDLE handle) {
	Proxy384LUSB_DisconnectFromModule(handle);
	LOG(INFO) << "[LWIR384] camera closed";
}