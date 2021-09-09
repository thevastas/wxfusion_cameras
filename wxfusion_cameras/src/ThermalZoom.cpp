#include "ThermalZoom.h"
#include <bitset>
using namespace boost;
void ThermalZoom::Close() {
	ThermalZoom::serial.close();
	return;
}

void ThermalZoom::ZoomIn(int speed) {
	speed = 4; // TODO speed fix on arduino
	unsigned char input[1]{ 0xC0 };
	input[0] = input[0] + speedarr[speed];
	asio::write(ThermalZoom::serial, asio::buffer(input));

	return;
}
void ThermalZoom::ZoomOut(int speed) {
	speed = 4; // TODO speed fix on arduino
	unsigned char input[1]{ 0x80 };
	
	input[0] = input[0] + speedarr[speed];
	//input[5] = m_speed[speed];
	asio::write(ThermalZoom::serial, asio::buffer(input));
	return;
}

void ThermalZoom::FocusIn(int speed) {
	speed = 4; // TODO speed fix on arduino
	unsigned char input[1]{ 0x0 };
	input[0] = input[0] + speedarr[speed];
	//input[4] = m_speed[speed];
	asio::write(ThermalZoom::serial, asio::buffer(input));
	return;
}

void ThermalZoom::FocusOut(int speed) {
	speed = 4; // TODO speed fix on arduino
	unsigned char input[1]{ 0x40 };
	input[0] = input[0] + speedarr[speed];
	//input[4] = m_speed[speed];
	asio::write(ThermalZoom::serial, asio::buffer(input));
	return;
}

void ThermalZoom::Stop() {
	unsigned char input[1]{ 0x00 };
	asio::write(ThermalZoom::serial, asio::buffer(input));
	return;
}
