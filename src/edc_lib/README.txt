This is the C++ example/library code package for communication with with 
Brainboxes Ethernet-attached data acquisition and control products, and is 
provided by Brainboxes Limited.  Examples in other programming languages are 
also available.

This is free and unencumbered software released into the public domain.

The file EDConsole.cpp compiles to an executable program which demonstrates 
the use of the Brainboxes_IO library, which is defined in the rest of the 
source files.
You can use the Brainboxes_IO library in your own programs, using the console 
program as an example of how it is used.

Visit http://www.brainboxes.com to see our range of Brainboxes Ethernet-
attached data acquisition and control products, and to check for updates to
this code package.

This is free and unencumbered software released into the public domain.

BUILDING
Edit the file 'porting.h' to suit your system before compiling.  If you are 
using a Microsoft compiler, you probably want to define USE_WINSOCK, and 
otherwise define USE_POSIX_SOCKETS.
For Unix-like systems, just type 'make' to build the 'EDConsole' executable.

RUNNING
The 'EDConsole' program takes up to three command-line arguments:
 Argument 1 (required) the IP address or host name of the ED device, e.g. 192.168.0.130
 Argument 2 (optional) the TCP port number to connect to - defaults to 9500
 Argument 3 (optional) the timeout for responses, in seconds - defaults to 1.0

TESTING
Tested with:
 * Microsoft Visual Studio 2010 v 10.0.40219.1 SP1Rel in Windows XP Professional SP3
 * gcc 4.7.3 in Cygwin 1.7.21(0.267/5/3)
 * gcc 4.6.3 in Ubuntu Linux, kernel 3.2.0-49
 * gcc 4.6.3 in Raspbian Linux, kernel 3.6.11+
