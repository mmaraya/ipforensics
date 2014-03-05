/**
 *  @file packet.h
 *  @brief Packet class definitions
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

#ifndef IPFORENSICS_PACKET_H_
#define IPFORENSICS_PACKET_H_

#include <stdint.h>
#include <iostream>  // NOLINT
#include "ipforensics/host.h"

/**
 *  @brief Model class for storing information about a single network packet,
 *         following the model-view-controller software design pattern
 *  @details Packet stores the media access control, Internet Protocol version 4,
 *           and Internet Protocol version 6 address information for both the
 *           network packet source and network packet destination.
 */
class Packet {
 private:
  /** Media access control address of the packet source */
  MACAddress mac_src_;

  /** Media access control address of the packet destination */
  MACAddress mac_dst_;

  /** Ethertype contained in the packet */
  uint16_t ether_type_ {};

  /** IPv4 address of the packet source */
  IPv4Address ipv4_src_;

  /** IPv4 address of the packet destination */
  IPv4Address ipv4_dst_;

  /** IPv6 address of the packet source */
  IPv6Address ipv6_src_;

  /** IPv6 address of the packet destination */
  IPv6Address ipv6_dst_;

 public:
  /**
   *  @brief Create a Packet instance using the supplied pcap pointer to the
   *         packet capture data
   */
  explicit Packet(const uint8_t *);

  /**
   *  @brief Does this Packet have IPv4 information
   *  @retval true if this Packet has IPv4 information, false otherwise
   */
  bool ipv4() const;

  /**
   *  @brief Does this Packet have IPv6 information
   *  @retval true if this Packet has IPv6 information, false otherwise
   */
  bool ipv6() const;

  /**
   *  @brief Accessor method for the mac_src_ property
   *  @retval MACAddress media access control address for the packet source
   */
  MACAddress mac_src() const;

  /**
   *  @brief Accessor method for the mac_dst_ property
   *  @retval MACAddress media access control address for the packet destination
   */
  MACAddress mac_dst() const;

  /**
   *  @brief Accessor method for the ether_type_ property
   *  @retval uint16_t ethertype for this Packet
   */
  uint16_t ether_type() const;

  /**
   *  @brief Accessor method for the ipv4_src_ property
   *  @retval IPv4Address source IPv4 address for this Packet
   */
  IPv4Address ipv4_src() const;

  /**
   *  @brief Accessor method for the ipv4_dst_ property
   *  @retval IPv4Address destination IPv4 address for this Packet
   */
  IPv4Address ipv4_dst() const;

  /**
   *  @brief Accessor method for the ipv6_src_ property
   *  @retval IPv6Address source IPv6 address for this Packet
   */
  IPv6Address ipv6_src() const;

  /**
   *  @brief Accessor method for the ipv6_dst_ property
   *  @retval IPv6Address destination IPv6 address for this Packet
   */
  IPv6Address ipv6_dst() const;
};

/**
 *  @brief Provide the std::string representation of a Packet by overloading the
 *         << operator for std::ostream
 *  @param out std::ostream output stream
 *  @param p Packet instance to display as an std::string
 *  @retval std::ostream address that contains the std::string representation of
 *          this Packet
 */
std::ostream &operator<<(std::ostream &out, const Packet &p);

#endif  // IPFORENSICS_PACKET_H_
