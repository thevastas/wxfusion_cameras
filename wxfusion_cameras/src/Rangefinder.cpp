#include "Rangefinder.h"
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

void Rangefinder::Close() {
	Rangefinder::serial.close();
	//if (Rangefinder::serial.is_open()) LOG(WARNING) << "Failed to close COM port!";
	//else	LOG(INFO) << "Closed port COM4";
	return;
}

double Rangefinder::Measure() {
	double result;
	char distance_hex;
	//MainWindow* comm = (MainWindow*)GetParent();
	//MainWindow* comm = (MainWindow*)m_parent->GetParent();
		//m_parent->GetParent();

	//comm->m_logpanel->m_logtext->AppendText("Measuring distance.. \n");
	//LOG(DEBUG) << "Starting rangefinder measurement";
	unsigned char input[4]{ 0x10,0x83,0x00,0x7D };
	asio::write(Rangefinder::serial, asio::buffer(input));
	//LOG(DEBUG) << "Serial writing done";
	unsigned char data[10];
	// asio::read will read bytes until the buffer is filled
	size_t nread = asio::read(Rangefinder::serial, asio::buffer(&data, 10));
	//LOG(DEBUG) << "Serial reading done";
	if (data[5] == 0xA0) {
		result = 0;
		//LOG(WARNING) << "Was not able to measure distance!";
	}
	else {
		distance_hex = (data[6] << 8) | (data[7]);
		result = distance_hex / 10;// Combine two bytes into one representing distance
	}
	


	//comm->m_logpanel->m_logtext->AppendText(wxString::Format(wxT("Distance: %.2f meters \n"),result));


	//LOG(INFO) << "Measured distance: " << std::dec << result;
	return result;
}

void Rangefinder::PointerOn() {
	//LOG(DEBUG) << "Starting pointer on";
	unsigned char input[4]{ 0x10,0x8F,0x01,0x70 };
	asio::write(Rangefinder::serial, asio::buffer(input));
	//LOG(DEBUG) << "Serial writing done";
	return;
}


void Rangefinder::PointerOff() {
	//LOG(DEBUG) << "Starting pointer on";
	unsigned char input[4]{ 0x10,0x8F,0x00,0x71 };
	asio::write(Rangefinder::serial, asio::buffer(input));
	//LOG(DEBUG) << "Serial writing done";
	return;
}