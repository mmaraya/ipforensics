/**
 *  @file device.h
 *  @brief Device class definitions
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

#ifndef IPFORENSICS_DEVICE_H_
#define IPFORENSICS_DEVICE_H_

#include <pcap/pcap.h>
#include <pcap/bpf.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "ipforensics/packet.h"

/**
 *  @brief Model class for storing information about a single network packet 
 *         capture device, following the model-view-controller software design 
 *         pattern.
 *  @details Device information is read from libpcap and contains a name, 
 *           description, loopback status, IPv4 network address (if IPv4), and
 *           IPv4 network mask (if IPv4).  The collection of Packet objects is
 *           also stored in the Device.
 *  @todo Since IPForensics can read packets from both a file and a network
 *        capture device, move the packets_ property to the IPForensics class.
 */
class Device {
 private:
  /** Name of the packet capture device */
  std::string name_;

  /** Description of the packet capture device */
  std::string desc_;

  /** Virtual network interface for diagnostics and troubleshooting */
  bool loopback_;

  /** IPv4 network address for this device */
  IPv4Address net_;

  /** IPv4 network mask for this device */
  IPv4Address mask_;

  /** Collection of Packets loaded from this device */
  std::vector<Packet> packets_;

 public:
  /**
   * @brief Accessor method for the name_ property
   * @retval std::string Name of the packet capture device
   */
  const std::string name() const;

  /**
   * @brief Accessor method for the desc_ property
   * @retval std::string Description of the packet capture device
   */
  const std::string desc() const;

  /**
   * @brief Accessor method for the loopback_ property
   * @retval bool True if loopback, false otherwise
   */
  const bool loopback() const;

  /**
   * @brief Accessor method for the net_ property
   * @retval IPv4Address IPv4 network address for this Device
   */
  const IPv4Address net() const;

  /**
   * @brief Accessor method for the mask_ property
   * @retval IPv4Address IPv4 network mask for this Device
   */
  const IPv4Address mask() const;

  /**
   * @brief Accessor method for the packets_ property
   * @retval std::vector<Packet> Collection of packets collected by this Device
   */
  const std::vector<Packet> packets();

  /**
   * @brief Mutator method for the name_ property
   * @param name Name of the packet capture device
   */
  void set_name(const std::string name);

  /**
   * @brief Mutator method for the desc_ property
   * @param desc Description of the packet capture device
   */
  void set_desc(const std::string desc);

  /**
   * @brief Mutator method for the loopback_ property
   * @param loopback True if device is loopback
   */
  void set_loopback(const bool loopback);

  /**
   * @brief Mutator method for the net_ property
   * @param net IPv4 network address for this device
   */
  void set_net(const IPv4Address net);

  /**
   * @brief Mutator method for the mask_ property
   * @param mask IPv4 network mask for this Device
   */
  void set_mask(const IPv4Address mask);

  /**
   * @brief Capture network packets from this Device
   * @param n Number of packets to capture
   * @retval int Actual number of packets captured
   */
  int capture(const int n);
};

/**
 *  @brief Provide the std::string representation of a Device by overloading the
 *         << operator for std::ostream
 *  @param out std::ostream output stream
 *  @param d Device instance to display as an std::string
 *  @retval std::ostream address that contains the std::string representation of
 *          this Device
 */
std::ostream &operator<<(std::ostream &out, const Device &d);

#endif  // IPFORENSICS_DEVICE_H_
