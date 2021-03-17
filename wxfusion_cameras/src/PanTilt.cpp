#include "PanTilt.h"
#include "MainWindow.h"
using namespace boost;


//void Rangefinder::Init(boost::asio::io_service& io){
//
//	asio::serial_port port(io);
//
//	port.open("COM4");
//	LOG(INFO) << "Opened port COM4"; //TODO make an actual check if the port is opened
//
//	port.set_option(asio::serial_port_base::baud_rate(115200));
//	port.set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));
//	port.set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none));
//	port.set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one));
//
//	LOG(INFO) << "Set the baud rate to 115200, parity: none, stop bits: one";
//
//		
//
//	return;
//}

void PanTilt::Close() {
	PanTilt::serial.close();
	//if (Rangefinder::serial.is_open()) LOG(WARNING) << "Failed to close COM port!";
	//else	LOG(INFO) << "Closed port COM4";
	return;
}

unsigned char PanTilt::Checksum(unsigned char array[7]) {
	unsigned char checksum = 0;
	for (int i = 1; i < 6; i++) {
		checksum = checksum + array[i];
	}
	return checksum;
}


void PanTilt::Up() {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x08, 0x00, 0x20, 0x00 };
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}
void PanTilt::Down() {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x10, 0x00, 0x20, 0x31 };
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::Left() {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x04, 0x20, 0x00, 0x25 };
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::Right() {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x02, 0x20, 0x00, 0x23 };
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::UpRight() {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x0A, 0x20, 0x20, 0x4B };
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::DownRight() {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x12, 0x20, 0x20, 0x53 };
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::UpLeft() {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x0C, 0x20, 0x20, 0x4D };
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::DownLeft() {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x14, 0x20, 0x20, 0x55 };
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::Stop() {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x00, 0x00, 0x00, 0x01 };
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}