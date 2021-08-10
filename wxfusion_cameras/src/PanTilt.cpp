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


void PanTilt::Up(int speed) {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x08, 0x00, 0x3F, 0x00 };
	input[5] = m_speed[speed];
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}
void PanTilt::Down(int speed) {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x10, 0x00, 0x04, 0x00 };
	input[5] = m_speed[speed];
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::Left(int speed) {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x04, 0x16, 0x00, 0x00 };
	input[4] = m_speed[speed];
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::Right(int speed) {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x02, 0x2F, 0x00, 0x00 };
	input[4] = m_speed[speed];
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::UpRight(int speed) {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x0A, 0x20, 0x20, 0x00 };
	input[4] = m_speed[speed];
	input[5] = m_speed[speed];
	input[6] = Checksum(input);

	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::DownRight(int speed) {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x12, 0x20, 0x20, 0x00 };
	input[4] = m_speed[speed];
	input[5] = m_speed[speed];
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::UpLeft(int speed) {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x0C, 0x20, 0x20, 0x00 };
	input[4] = m_speed[speed];
	input[5] = m_speed[speed];
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::DownLeft(int speed) {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x14, 0x20, 0x20, 0x00 };
	
	input[4] = m_speed[speed];
	input[5] = m_speed[speed];
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::Stop() {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x00, 0x00, 0x00, 0x01 };
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::SetPreset(int pos) {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x03, 0x00, 0x01, 0x01 };
	input[5] = pos;
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

void PanTilt::Preset(int pos) {
	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x07, 0x00, 0x01, 0x01 };
	input[5] = pos;
	input[6] = Checksum(input);
	asio::write(PanTilt::serial, asio::buffer(input));
	return;
}

//int PanTilt::QueryPan() {
//	unsigned char data[10];
//	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x53, 0x00, 0x00, 0x01 };
//	//input[5] = pos;
//	input[6] = Checksum(input);
//	asio::write(PanTilt::serial, asio::buffer(input));
//	size_t nread = asio::read(PanTilt::serial, asio::buffer(&data, 10));
//	int a = data[5];
//	int b = data[6];
//	return 0;
//}
//
//
//int PanTilt::QueryTilt() {
//	unsigned char data[10];
//	unsigned char input[7]{ 0xFF,0x01, 0x00, 0x55, 0x00, 0x00, 0x01 };
//	//input[5] = pos;
//	input[6] = Checksum(input);
//	asio::write(PanTilt::serial, asio::buffer(input));
//	size_t nread = asio::read(PanTilt::serial, asio::buffer(&data, 10));
//	int a = data[5];
//	int b = data[6];
//	return 0;
//}