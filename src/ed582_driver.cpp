/**
 * @file ed582_driver.cpp
 * @author Robert Raye (robert.raye@nrel.gov)
 * @brief ros node for acquiring rtd temperatures from the brainboxes Ed582
 * @version 0.1
 * @date 2024-10-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <chrono>
#include "edc_lib/porting.h"
#include "edc_lib/EDDeviceTCP.h"
#include "edc_lib/exceptions.h"
#include "rclcpp/rclcpp.hpp"
#include "modaq_messages/msg/ed582.hpp"
using namespace std::chrono_literals;

class ED582_TCP_Reader : public rclcpp::Node
{
public:
  ED582_TCP_Reader() : Node("eds582_node"), iodev()
  { 
    this->declare_parameter<std::string>("IPAddress", "192.168.40.60");
    IPAddress = this->get_parameter("IPAddress").as_string();

    this->declare_parameter<std::string>("RTDtopic", "/rtd");    
    std::string RTDtopic = this->get_parameter("RTDtopic").as_string();

    this->declare_parameter<int>("sampleRateMS", 1000);    
    int sampleRateMS = this->get_parameter("sampleRateMS").as_int();

    publisher_ = this->create_publisher<modaq_messages::msg::Ed582>(RTDtopic, 10);
    timer_ = this->create_wall_timer(std::chrono::milliseconds(sampleRateMS), std::bind(&ED582_TCP_Reader::timer_callback, this));

    //make IP Addr and Port ROS Parameters here
    try
    {
      iodev.SetIPandPort(IPAddress,ed_port);
      iodev.SetTimeout(timeout);
      iodev.Connect();
      std::cout << "connected.\n" << std::flush;
    }
    catch (Brainboxes_IO::Exception *e)
    {
      std::cerr << "An error occurred: " << e->message() << "\n" << std::flush;
    }
    catch (...)
    {
      std::cerr << "An unexpected exception occurred.\n" << std::flush;
    }
  }

  ~ED582_TCP_Reader()
  {
    iodev.Disconnect();
  }

private:
  Brainboxes_IO::EDDeviceTCP iodev;  // The TCP device handler
  double timeout = 1.0;
  uint64_t timestamp_ns;
  std::string data;
  std::string IPAddress;
  std::string ed_port = "9500";
  
  rclcpp::Publisher<modaq_messages::msg::Ed582>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  modaq_messages::msg::Ed582 msg;
  std::chrono::time_point<std::chrono::system_clock> time;
  
  void timer_callback()
  {
    //std::string devname = iodev.SendCommand("$01M");
    //std::cout << "whoami '$01M' is " << devname << std::endl;
    //std::string data = iodev.SendCommand("$016");
    //std::cout << "Active Channels '$016' is " << data << std::endl;
    
    data = iodev.SendCommand("#010"); // ch0
    //float ch0 = std::stof(data.substr(1, data.length()));  // substr to remove the '>' that is returned, convert to float
    //std::cout << "Ch0 '#010' is " << ch0 << std::endl;
    msg.ch0 = std::stof(data.substr(1, data.length()));
    data = iodev.SendCommand("#011"); // ch1
    msg.ch1 = std::stof(data.substr(1, data.length()));
    data = iodev.SendCommand("#012"); // ch2
    msg.ch2 = std::stof(data.substr(1, data.length()));
    data = iodev.SendCommand("#013"); // ch3
    msg.ch3 = std::stof(data.substr(1, data.length()));
    msg.header.stamp = now();

    publisher_->publish(msg);
  }
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ED582_TCP_Reader>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}