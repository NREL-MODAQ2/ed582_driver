/*
This file is part of the C++ example/library code for communication with with 
Brainboxes Ethernet-attached data acquisition and control products, and is 
provided by Brainboxes Limited.  Examples in other programming languages are 
also available.
Visit http://www.brainboxes.com to see our range of Brainboxes Ethernet-
attached data acquisition and control products, and to check for updates to
this code package.

This is free and unencumbered software released into the public domain.
*/

#pragma once
#ifdef USE_STD_MUTEX
	#include <mutex>
#endif
#include <string>

namespace Brainboxes_IO 
{

	class EDDevice
	{
	public:
		EDDevice(void);
		virtual ~EDDevice(void);
		virtual bool IsConnected() = 0;
        virtual void SetTimeout(double timeout) = 0;
		void Connect();
		void Disconnect();

		std::string SendCommand(std::string);

	protected:
        //-- these functions will be overridden in the child classes EDDeviceTCP and EDDeviceSerial --//
		virtual void do_connect() = 0;
		virtual std::string sendCommand(std::string command) = 0;
		virtual void disconnect() = 0;

	private:
#ifdef USE_STD_MUTEX
		std::mutex m_command_mutex;
#endif
	};

    // New line character used by the ASCII DCON protocol to signal end of message
	static const std::string newLine = "\r";
} 
