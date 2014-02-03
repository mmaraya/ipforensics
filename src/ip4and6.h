/**
 *  @file ipforensics.h
 *  @brief IPForensics class definitions
 */

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Michael Maraya
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <iostream>
#include <vector>
#include <pcap/pcap.h>
#include <set>
#include "device.h"

class IPForensics {
 private:
  std::vector<Device> devices_;
  std::set<Host> hosts_;
  std::string device_;
  std::string filename_;
  int packet_count_;
  std::vector<Packet> packets_;
  void add_host(MACAddress, IPv4Address, IPv6Address);
  void update_host(std::set<Host>::iterator, IPv4Address, IPv6Address);

  /**
   *  @brief Remove broadcast, multicast and non-local hosts from 
   *         IPForensics::hosts_
   *  @param net IPv4 network address used by the capture device
   *  @param mask IPv4 network mask used by the capture device
   */
  void clean_hosts(IPv4Address* net, IPv4Address* mask);
 public:
  std::vector<Device> devices();
  std::set<Host> hosts();
  std::string device();
  std::string filename();
  int packet_count();
  std::vector<Packet> packets();
  void set_device(std::string);
  void set_filename(std::string);
  void set_packet_count(int);
  
  /**
   *  @brief Queries the system for all available packet capture devices and
   *         enters them into IPForensics::devices_
   */
  void load_devices();

  /**
   *  @brief Extracts all unique hosts from the packets captured on the supplied
   *         Device and enters them into IPForensics::hosts_
   *  @param filename User-supplied filename of the packet capture file to read
   */
  void load_hosts(Device);

  /**
   *  @brief Reads all unique hosts from the user-supplied packet capture file
   *         and enters them into IPForensics::hosts_
   *  @param filename User-supplied filename of the packet capture file to read
   */
  void load_hosts(std::string filename);
};
