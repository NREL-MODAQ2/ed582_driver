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

#include "porting.h"
#include "exceptions.h"
#include "EDDeviceTCP.h"

#include <stdlib.h>
#include <iostream>
#include <string>

#ifdef USE_WINSOCK
#include <winsock2.h> 
#pragma comment(lib, "Ws2_32.lib")
#define PROCESS_CLEANUP { WSACleanup(); }
#else
#define PROCESS_CLEANUP { }
#endif

int main(int argc, char **argv)
{
	std::string ed_ipaddr;
	std::string ed_port = "9500";
    double timeout = 1.0;

	if (argc < 2) {
		std::cout << "usage: " << argv[0] << " <IP address> [<port number>] [<timeout>]\n";
		return 1;
	}
	
#ifdef USE_WINSOCK
	// If we're using WinSock, it needs to be initialised once per application/process
	{
		WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD(2, 2);
 
		if (WSAStartup(wVersionRequested, &wsaData) != 0) {
			// throw new Brainboxes_IO::Exception("Failed to initialise WinSock");
			std::cerr << "Failed to initialise WinSock.\n" << std::flush;
			return 1;
		}
	}
#endif

	// get the arguments from the command line
	ed_ipaddr = argv[1];

	if (argc >= 3)
		ed_port = argv[2];

	if (argc >= 4)
		timeout = atof(argv[3]);

	std::cout << "Will attempt to connect to IP address '" << ed_ipaddr << "', port number " << ed_port << "...\n" << std::flush;

	try {
		// create the Brainboxes_IO::EDDeviceTCP object which encapsulates the connection to the ED device
		Brainboxes_IO::EDDeviceTCP iodev(ed_ipaddr, ed_port);
        iodev.SetTimeout(timeout);
        
		// open the connection
		iodev.Connect();
		std::cout << "connected.\n" << std::flush;

		// an example non-interactive communication
		std::string devname = iodev.SendCommand("$01M");
		std::cout << "Response to '$01M' is '" << devname << "'\n";
        
		// a simple interactive terminal
		std::cout << "\nEnter ASCII commands at the prompt below: enter a blank command to exit.\n";
		while (!std::cin.eof()) {
			// get an ASCII command from the user
			std::string command;
			std::cout << ">" << std::flush;
			std::getline (std::cin, command);
			if (command.length()==0)
				break;
			// send the command to the ED device and display the response
			std::string response = iodev.SendCommand(command);
			std::cout << response << "\n" << std::flush;
		}

		// close the connection
        iodev.Disconnect();
	}
	catch (Brainboxes_IO::Exception *e) {
		// error handling: exceptions thrown by the Brainboxes_IO library
		std::cout << std::flush;
		std::cerr << "An error occurred: " << e->message() << "\n" << std::flush;
		PROCESS_CLEANUP
		return 1;
	}
	catch (...) {
		// error handling: exceptions thrown by something other than the Brainboxes_IO library
		std::cout << std::flush;
		std::cerr << "An unexpected exception occurred.\n" << std::flush;
		PROCESS_CLEANUP
		return 1;
	};

	// std::cout << "\nClean exit\n";
	PROCESS_CLEANUP

	return 0;
}