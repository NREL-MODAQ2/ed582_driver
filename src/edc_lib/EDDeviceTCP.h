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
#include "EDDevice.h"
#include <string>
#include "porting.h"

#ifdef USE_POSIX_SOCKETS
#include <sys/socket.h>
#endif

#ifdef USE_WINSOCK
#include <winsock2.h> 
#endif

namespace Brainboxes_IO 
{
	class EDDeviceTCP : public EDDevice
	{
	public:
        /// <summary>
        /// Create a TCP Connection to a Brainboxes EDDevice
        /// Use Brainboxes Boost.IO to find out the IP address of the Brainboxes EDDevice on your network
        /// </summary>
        /// <param name="IP">The IP address of the EDDevice to connect to, defaults to 192.168.127.255, which will be the case if the device is on a network without DHCP and the connecting computer is on the same subnet</param>
        /// <param name="Port">The TCP IP Port number, defaults to 9500</param>
		EDDeviceTCP();
        void SetTimeout(double timeout);
		bool IsConnected();
		void SetIPandPort(std::string ip , std::string port );
		~EDDeviceTCP(void);

	protected:
		
		void do_connect();
		std::string sendCommand(std::string command);
		void disconnect();

	private:
		std::string m_ip;
		std::string m_port;
		int m_socket;
		fd_set socket_fd;
		struct timeval m_timeout;
	};

}