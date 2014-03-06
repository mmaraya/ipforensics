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

#ifndef IPFORENSICS_IP4AND6_H_
#define IPFORENSICS_IP4AND6_H_

#include <stdint.h>
#include <pcap/pcap.h>
#include <set>
#include <string>
#include <vector>
#include "ipforensics/device.h"

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
  std::string in_file_;

  /**
   *  @brief Name of the file to write host summary to
   */
  std::string out_file_;

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
   *  @brief Accessor method for the in_file_ property
   *  @retval std::string name of the file to read packets from
   */
  std::string in_file() const;

  /**
   *  @brief Accessor method for the out_file_ property
   *  @retval std::string name of the file to write the host summary to
   */
  std::string out_file() const;

  /**
   *  @brief Accessor method for the packet_count_ property
   *  @retval int number of packets to read from the network or file
   */
  int packet_count() const;

  /**
   *  @brief Accessor method for the packets_ property
   *  @retval std::vector packets read from the capture device or file
   */
  std::vector<Packet> packets();

  /**
   *  @brief Mutator method for the device_ property
   *  @param device Device instance to read packets from
   */
  void set_device(std::string device);

  /**
   *  @brief Mutator method for the in_file property
   *  @param in_file user-supplied libpcap-formatted file to load packets from
   */
  void set_in_file(std::string in_file);

  /**
   *  @brief Mutator method for the out_file property
   *  @param out_file user-supplied filename to write host summary to
   */
  void set_out_file(std::string out_file);

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

  /**
   *  @brief Write host summary results to an either the screen or a file
   */
  void results();

  /**
   * @brief friend function from the Device class for capture network packets
   * @details IPForensics::packets_ stores the collection of packet and Device
   *          needs to modify it 
   * @param n Number of packets to capture
   * @retval int Actual number of packets captured
   */
  friend int Device::capture(const int n);
};

/**
 *  @brief IPForensics namespace for library-wide constants
 */
namespace ipf {

  /** program name */
  const std::string kProgramName {"ipforensics"};

  /** program major revision number */
  const int kMajorVersion {0};

  /** program minor revision number */
  const int kMinorVersion {9};

  /** ethernet frame snapshot length */
  const int kSnapLength {256};

  /** number of milliseconds to wait for each network packet */
  const int kTimeout {1000};

  /** number of segments in a MAC address */
  const int kLengthMAC {6};

  /** number of segments in an IPv4 address */
  const int kLengthIPv4 {4};

  /** number of segments in an IPv6 address */
  const int kLengthIPv6 {16};

  /** MAC source address packet offset */
  const int kOffsetMACSrc {6};

  /** MAC destination address packet offset */
  const int kOffsetMACDst {0};

  /** ethertype packet offset */
  const int kOffsetEtherType {12};

  /** IPv4 source address packet offset */
  const int kOffsetIPv4Src {26};

  /** IPv4 destination address packet offset */
  const int kOffsetIPv4Dst {30};

  /** IPv6 source address packet offset */
  const int kOffsetIPv6Src {22};

  /** IPv6 destination address packet offset */
  const int kOffsetIPv6Dst {38};

  /** ethertype for IPv4 */
  const uint16_t kEtherTypeIPv4 {0x0800};

  /** ethertype for IPv6 */
  const uint16_t kEtherTypeIPv6 {0x86DD};

  /** ethertype for Address Resolution Protocol (ARP) */
  const uint16_t kEtherTypeARP {0x0806};

  /** ARP IPv4 source address packet offset */
  const int kOffsetARPIPv4 {28};

  /** IPv4 prefix for multicasts */
  const uint8_t kMulticastIPv4 {0xE};

  /** IPv4 broadcast address */
  const IPv4Address kBroadcastIPv4 {std::vector<uint8_t> (4, 0xFF)};

  /** MAC broadcast address */
  const MACAddress kBroadcastMAC {std::vector<uint8_t> (6, 0xFF)};

  /** output header for comma-separated values file */
  const std::string kHeaderCSV {"MAC Address,IPv4 Address,IPv6 Address"};

  /** output header for console display */
  const std::string kHeader {"MAC Address       IPv4 Address    IPv6 Address\n"
    + std::string(17, '=') + ' ' + std::string(15, '=') + ' '
    + std::string(39, '=')};
}  // namespace ipf

#endif  // IPFORENSICS_IP4AND6_H_
