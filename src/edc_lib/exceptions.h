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
#include <string>

// A simple set of exception classes so that when when the Brainboxes_IO library hits a problem it can be handled by the calling application

namespace Brainboxes_IO 
{
	class Exception
	{
	public:
		Exception(std::string message) {
			m_message = message;
		}

		Exception(const char* message) {
			m_message = std::string(message);
		}

		virtual std::string message() {
			return m_message;
		}

		virtual ~Exception() {}

	private:
		std::string m_message;
	};

	class InvalidOperationException : public Exception
	{
	public:
		InvalidOperationException(std::string message) : Exception(message) { }
		InvalidOperationException(const char* message) : Exception(message) { }
	};

	class InvalidArgumentException : public Exception
	{
	public:
		InvalidArgumentException(std::string message) : Exception(message) { }
		InvalidArgumentException(const char* message) : Exception(message) { }
	};

	class CommsException : public Exception
	{
	public:
		CommsException(std::string message) : Exception(message) { }
		CommsException(const char* message) : Exception(message) { }
	};

	class ResponseTimeoutException : public Exception
	{
	public:
		ResponseTimeoutException(std::string message) : Exception(message) { }
		ResponseTimeoutException(const char* message) : Exception(message) { }
	};
}
