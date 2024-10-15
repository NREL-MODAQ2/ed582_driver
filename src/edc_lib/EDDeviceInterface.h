#pragma once
#include <string>

namespace Brainboxes_IO 
{
    /// <summary>
    /// Interface to a Brainboxes ED Device
    /// </summary>
	class EDDeviceInterface
	{
	public:

		/// <summary>
        /// Open Connection to EDDevice, must be called before SendCommand
        /// </summary>
        /// <returns></returns>
		virtual bool Connect() = 0;

        /// <summary>
        /// Disconnect from EDDevice, will be automatically called when class is disposed
        /// </summary>
		virtual void Disconnect() = 0;

        /// <summary>
        /// Send an ASCII DCON command to an EDDeivce
        /// </summary>
        /// <param name="command">ASCII DCON command see Brainboxes reference manual for details</param>
        /// <returns></returns>
		virtual std::string SendCommand(std::string) = 0;

        /// <summary>
        /// Check if Conenction to the EDDevice is open
        /// </summary>
		virtual bool IsConnected() = 0;

		EDDeviceInterface(void) {}
		virtual ~EDDeviceInterface(void) {}
	};
}
