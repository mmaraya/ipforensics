/**
 *  @file ip4and6.h
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
#include <pcap/pcap.h>
#include <set>
#include <vector>
#include "device.h"

/**
 *  @brief Main controller class for the IPForensics library, following the 
 *         model-view-controller software design pattern
 *  @details IPForensics ties all of the classes in this program together, 
 *           initializing the data sources (either packet capture device or
 *           libpcap-format file), reading the packets, and extracting hosts.
 */
class IPForensics {
 private:
  
  /**
   *  @brief Collection of network capture devices available from the system
   *  @details IPForensics::load_devices(Device) must be called to populate this
   *           variable.  If IPForensics is used to read a packet capture file,
   *           this variable will not be used and may be kept empty.
   */
  std::vector<Device> devices_;

  /**
   *  @brief Collection of hosts, uniquely identified by their MAC addresses
   */
  std::set<Host> hosts_;
 
  /**
   *  @brief Name of the network capture device to read packets from
   */
  std::string device_;
  
  /**
   *  @brief Name of the file to read packets from
   *  @details File must follow the libpcap file format
   */
  std::string filename_;
  
  /**
   *  @brief Number of packets to read from the network or file
   *  @details If reading a file, a value of 0 means read all packets
   */
  int packet_count_;

  /**
   *  @brief Packets from the capture device or libpcap file are stored in this
   *         collection
   */
  std::vector<Packet> packets_;
  
  /**
   *  @brief Adds a new Host to IPForensics::hosts_
   *  @param mac MACAddress for this new host, this is a mandatory value
   *  @param ipv4 IPv4Address for this new host, if known
   *  @param ipv6 IPv6Address for this new host, if known
   */
  void add_host(MACAddress mac, IPv4Address ipv4, IPv6Address ipv6);
  
  /**
   *  @brief Sets the IPv4 and/or IPv6 addresses of an existing Host in hosts_
   *  @param it iterator pointing to the existing Host in hosts_
   *  @param ipv4 IPv4Address associated with this host
   *  @param ipv6 IPv6Address associated with this host
   */
  void update_host(std::set<Host>::iterator it, IPv4Address ipv4,
                   IPv6Address ipv6);

  /**
   *  @brief Remove broadcast, multicast and non-local hosts from 
   *         IPForensics::hosts_
   *  @param net IPv4 network address used by the capture device
   *  @param mask IPv4 network mask used by the capture device
   */
  void clean_hosts(IPv4Address* net, IPv4Address* mask);
 
 public:
  
  /**
   *  @brief Accessor method for the devices_ property
   *  @retval std::vector of network capture devices available from the system
   */
  std::vector<Device> devices() const;

  /**
   *  @brief Accessor method for the hosts_ property
   *  @retval std::set of hosts, uniquely identified by their MAC addresses
   */
  std::set<Host> hosts() const;
  
  /**
   *  @brief Accessor method for the device_ property
   *  @retval std::string name of the network capture device being used
   */
  std::string device() const;

  /**
   *  @brief Accessor method for the filename_ property
   *  @retval std::string name of the file to read packets from
   */
  std::string filename() const;
  
  /**
   *  @brief Accessor method for the packet_count_ property
   *  @retval int number of packets to read from the network or file
   */
  int packet_count() const;
  
  /**
   *  @brief Accessor method for the packets_ property
   *  @retval std::vector packets read from the capture device or file
   */
  std::vector<Packet> packets() const;
  
  /**
   *  @brief Mutator method for the device_ property
   *  @param device Device instance to read packets from
   */
  void set_device(std::string device);

  /**
   *  @brief Mutator method for the filename_ property
   *  @param filename user-supplied libpcap-formatted file to load packets from
   */
  void set_filename(std::string filename);
  
  /**
   *  @brief Mutator method for the packet_count_ property
   *  @param count number of packets to read from the network or file
   *  @details If reading a file, a value of 0 is taken to mean read all packets
   */
  void set_packet_count(int count);
  
  /**
   *  @brief Queries the system for all available packet capture devices and
   *         enters them into IPForensics::devices_
   */
  void load_devices();

  /**
   *  @brief Extracts all unique hosts from the packets captured on the supplied
   *         Device and enters them into IPForensics::hosts_
   *  @param device Network packet device to read packets and extract hosts from
   */
  void load_hosts(Device device);

  /**
   *  @brief Reads all unique hosts from the user-supplied packet capture file
   *         and enters them into IPForensics::hosts_
   *  @param filename User-supplied filename of the packet capture file to read
   */
  void load_hosts(std::string filename);
};
