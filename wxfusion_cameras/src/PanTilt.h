#pragma once
//
#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include "Common.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <boost/thread.hpp>

class PanTilt : public wxFrame
{
private:
	boost::asio::io_service io;
	boost::asio::serial_port serial;
	unsigned char m_speed[6]{ 0x00, 0x04, 0x11, 0x20, 0x2F, 0x3F };

	//Speed                     m_speed{ Empty };
public:
	PanTilt(std::string port, unsigned int baud_rate)
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
	unsigned char Checksum(unsigned char array[7]);
	void Up(int speed);
	void Down(int speed);
	void Left(int speed);
	void Right(int speed);
	void UpLeft(int speed);
	void UpRight(int speed);
	void DownLeft(int speed);
	void DownRight(int speed);
	void Stop();
	void SetPreset(int pos);
	void Preset(int pos);
	//int QueryPan();
	//int QueryTilt();
};
