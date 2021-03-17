#pragma once
//
#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include "Common.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <boost/thread.hpp>

class Rangefinder : public wxFrame
{
private:
	boost::asio::io_service io;
	boost::asio::serial_port serial;
public:
	Rangefinder(std::string port, unsigned int baud_rate)
		: io(), serial(io, port)
	{
		serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
		serial.set_option(boost::asio::serial_port_base::character_size(8));
		serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
		serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
		serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
			}

	//asio::io_service io;
	//void Init(boost::asio::io_service& io);
	void Close();
	void PointerOn();
	void PointerOff();
	double Measure();
};
