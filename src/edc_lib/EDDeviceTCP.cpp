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

#include "EDDeviceTCP.h"
#include "exceptions.h"
#include "porting.h"

#ifdef USE_POSIX_SOCKETS
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <unistd.h> 
#include <string.h> 
#endif

#ifdef USE_WINSOCK
#include <winsock2.h> 
#include <ws2tcpip.h> 
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#endif

namespace Brainboxes_IO 
{

/*  These constants are useful, but appear to be specific to */
/*  Winsock; so, I define them here and use them as if they */
/*  were standard */
#ifndef USE_WINSOCK
	const int INVALID_SOCKET = ~0;
	const int SOCKET_ERROR = -1;
#endif

	EDDeviceTCP::EDDeviceTCP(){

	}

	void EDDeviceTCP::SetIPandPort(std::string ip, std::string port)
	{
		m_ip = ip;
		m_port = port;
		m_socket = INVALID_SOCKET;
		m_timeout.tv_sec = 1;
		m_timeout.tv_usec = 0;
	}
    
    void EDDeviceTCP::SetTimeout(double timeout) {
		if (timeout <= 0)
			throw new InvalidArgumentException("Negative or zero timeout value specified for EDDeviceTCP object");
		m_timeout.tv_sec = long(timeout);
		m_timeout.tv_usec = long((timeout - m_timeout.tv_sec) * 1000000.0);
        //~ printf("timeout set to %ld.%06ld\n", (long int) m_timeout.tv_sec, (long int) m_timeout.tv_usec);
	}

	bool EDDeviceTCP::IsConnected() {
		return m_socket != INVALID_SOCKET;
	}

	void EDDeviceTCP::do_connect() {
		struct addrinfo *server_addrinfo = NULL, addrinfo_hints;

		memset( &addrinfo_hints, 0, sizeof(addrinfo_hints) );
		addrinfo_hints.ai_family = AF_UNSPEC;
		addrinfo_hints.ai_socktype = SOCK_STREAM;
		addrinfo_hints.ai_protocol = IPPROTO_TCP;
		if (getaddrinfo(m_ip.data(), m_port.data(), &addrinfo_hints, &server_addrinfo) != 0)
			throw new CommsException("Failed to resolve IP address");
 
		// Attempt to connect to an address until one succeeds
		try {
			for (addrinfo *ptr=server_addrinfo; ptr != NULL ;ptr=ptr->ai_next) {

				// Create a SOCKET for connecting to ED device
				m_socket = socket(ptr->ai_family, ptr->ai_socktype, 
					ptr->ai_protocol);
				if (m_socket == INVALID_SOCKET)
					throw new CommsException("Failed to create a tcp socket");

				// Connect to ED device.
				if (connect( m_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
#ifdef USE_POSIX_SOCKETS
					close(m_socket);
#endif
#ifdef USE_WINSOCK
					closesocket(m_socket);
#endif
					m_socket = INVALID_SOCKET;
					continue;
				}
				break;
			}
		} 
		catch (...) {
			freeaddrinfo(server_addrinfo);
			throw;
		}

		freeaddrinfo(server_addrinfo);

		if (m_socket == INVALID_SOCKET)
			throw new CommsException("Failed to connect to ED device");

		// prepare the fd_set structure for use by the select() function later
		FD_ZERO(&socket_fd);
		FD_SET(m_socket, &socket_fd);
	}
	
	std::string EDDeviceTCP::sendCommand(std::string command) {
		char buff[255];


		std::string terminated_command = command + newLine;

		// send the command out to the ED device
		if(send(m_socket, terminated_command.data(), terminated_command.length(), 0) == -1) {
			throw new CommsException("Failed sending message to ED device");
		}

		// use select() to wait until the socket is ready for reading
		fd_set read_fds = socket_fd;
		fd_set except_fds = socket_fd;
		select(m_socket+1, &read_fds, NULL, &except_fds, &m_timeout);
		if (FD_ISSET(m_socket, &except_fds)) {
			throw new CommsException("Error on connection to ED device");
		}
		if (!FD_ISSET(m_socket, &read_fds)) {
			// we've reached the timeout without seeing a response from the socket
			// uncomment the following line if you want an exception to be thrown in this situation
			//throw new ResponseTimeoutException("Timed out waiting for a response to the command '" + command + "'");

			// return an empty string as the response
			return std::string();
		}

		// read the response from the socket
		size_t size = recv(m_socket, buff, sizeof(buff), 0);
 
		// check for error conditions
		if (size == 0) {
			throw new CommsException("Connection closed by ED device");
		} 
		if (size <= 0) {
			throw new CommsException("Error receiving data from ED device");
		} 

		// strip trailing NL/CR characters from response
		while (size>0 && (buff[size-1]==0x0d || buff[size-1]==0x0a))
			size --;

		// return the response
		return std::string(buff, size);

	}

	void EDDeviceTCP::disconnect() {
		if (m_socket == INVALID_SOCKET)
			return;

		// close the socket connection
#ifdef USE_POSIX_SOCKETS
		shutdown(m_socket, SHUT_RDWR);
		close(m_socket);
#endif
#ifdef USE_WINSOCK
		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
#endif
		
		// mark this object as having a closed connection
		m_socket = INVALID_SOCKET;
	}

	EDDeviceTCP::~EDDeviceTCP() {
		disconnect();
	}

}