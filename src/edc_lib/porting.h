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

// Socket library selection: you must define one and only one of these, to select the network interfacing method
// If you are using a Microsoft compiler, you probably want to define USE_WINSOCK, and otherwise define USE_POSIX_SOCKETS
#define USE_POSIX_SOCKETS
//#define USE_WINSOCK

// Define USE_STD_MUTEX if you want to use the standard library's mutex class for resource locking
// This is not needed if each Brainboxes_IO::EDDevice is only used from a single thread
//#define USE_STD_MUTEX