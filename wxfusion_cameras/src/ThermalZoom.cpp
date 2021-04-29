#include "ThermalZoom.h"
#include <bitset>
using namespace boost;
void ThermalZoom::Close() {
	ThermalZoom::serial.close();
	//if (Rangefinder::serial.is_open()) LOG(WARNING) << "Failed to close COM port!";
	//else	LOG(INFO) << "Closed port COM4";
	return;
}


void ThermalZoom::ZoomIn(int speed) {
	//unsigned char input = 0x84;
	unsigned char input[1]{ 0x84 };
	//input[5] = m_speed[speed];
	asio::write(ThermalZoom::serial, asio::buffer(input));

	return;
}
void ThermalZoom::ZoomOut(int speed) {
	unsigned char input[1]{ 0xC4 };
	//input[5] = m_speed[speed];
	asio::write(ThermalZoom::serial, asio::buffer(input));
	return;
}

void ThermalZoom::FocusIn(int speed) {
	unsigned char input[1]{ 0x4 };
	//input[4] = m_speed[speed];
	asio::write(ThermalZoom::serial, asio::buffer(input));
	return;
}

void ThermalZoom::FocusOut(int speed) {
	unsigned char input[1]{ 0x44 };
	//input[4] = m_speed[speed];
	asio::write(ThermalZoom::serial, asio::buffer(input));
	return;
}

void ThermalZoom::Stop() {
	unsigned char input[1]{ 0x00 };
	asio::write(ThermalZoom::serial, asio::buffer(input));
	return;
}
