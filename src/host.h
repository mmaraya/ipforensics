/**
 *  @file host.h
 *  @brief Network host class definitions
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
#include <ostream>
#include <string>
#include "address.h"

/**
 *  @brief Host is a model class from the model-view-controller software design 
 *         pattern that stores information about a network host
 *  @details Host stores the media access control, Internet Protocol version 4,
 *           and Internet Protocol version 6 address information for a network 
 *           node.
 */
class Host {
 private:

  /** Media access control address */
  MACAddress mac_;

  /** Internet Protocol version 4 address */
  IPv4Address ipv4_;

  /** Internet Protocol version 6 address */
  IPv6Address ipv6_;

 public:
  
  /**
   *  @brief Construct a new Host from the supplied MACAddress
   *  @param mac MACAddress of the new Host to create
   *  @details This is the normal way to create a Host since the MAC address is
   *           used to uniquely identify a network node
   */
  Host(const MACAddress mac);

  /**
   *  @brief Construct a new Host from the supplied MAC, IPv4 and IPv6 addresses
   *  @param mac MACAddress of the new Host to create
   *  @param ipv4 IPv4Address of the new Host to create
   *  @param ipv6 IPv6Address of the new Host to create
   */
  Host(const MACAddress mac, const IPv4Address ipv4, const IPv6Address ipv6);

  MACAddress mac() const;
  IPv4Address ipv4() const;
  IPv6Address ipv6() const;
  void set_ipv4(const IPv4Address);
  void set_ipv6(const IPv6Address);
};

/**
 *  @brief Overload the < binary infix comparison operator
 *  @details IPForensics::hosts_ stores Hosts as an std::set so this class must
 *           overload the < operator so that the Hosts sort correctly
 *  @todo Overload the >, ==, !=, <=, and >= operators as well
 */
bool operator<(const Host& lhs, const Host& rhs);

std::ostream &operator<<(std::ostream &out, const Host &h);
