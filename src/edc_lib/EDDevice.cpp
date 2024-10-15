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

#include "EDDevice.h"
#include "exceptions.h"

namespace Brainboxes_IO 
{
	EDDevice::EDDevice(void)
	{
	}

	void EDDevice::Connect()
    {
        if (IsConnected()) 
			return;
        // Debug.WriteLine("Connecting");
        do_connect();
    }

    std::string EDDevice::SendCommand(std::string command)
    {
        if (!IsConnected()) 
			throw new Brainboxes_IO::InvalidOperationException("Brainboxes EDDevice not connected");

        // in multi-threaded code, the following code block must be locked to ensure the response 
        // correlates to the command sent and is not a response to another thread calling sendcommand 
        {
#ifdef USE_STD_MUTEX
			std::lock_guard<std::mutex> lock(m_command_mutex);
#endif
            // Debug.WriteLine(" <== " + command);
            std::string response = sendCommand(command);
            // Debug.WriteLine(" ==> " + response);
            return response;
        }
    }

	void EDDevice::Disconnect()
    {
        if (!IsConnected()) 
			return;
        // Debug.WriteLine("Disconnecting");
        disconnect();
    }

    EDDevice::~EDDevice()
    {
        // Disconnect(); // Can't do this, because the destructor for the derived class is called first
    }

}