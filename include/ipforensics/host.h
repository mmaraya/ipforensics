/**
 *  @file host.h
 *  @brief Host class definitions
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

#ifndef IPFORENSICS_HOST_H_
#define IPFORENSICS_HOST_H_

#include <string>
#include "ipforensics/address.h"

/**
 *  @brief Model class for storing information about a single network node, 
 *         following the model-view-controller software design pattern
 *  @details Host stores the media access control, Internet Protocol version 4,
 *           and Internet Protocol version 6 address information for a single 
 *           network node.
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
   *  @brief Construct an empty Host
   */
  Host();

  /**
   *  @brief Construct a new Host from the supplied MACAddress
   *  @param mac MACAddress of the new Host to create
   *  @details This is the normal way to create a Host since the MAC address is
   *           used to uniquely identify a network node
   */
  explicit Host(const MACAddress mac);

  /**
   *  @brief Construct a new Host from the supplied MAC, IPv4 and IPv6 addresses
   *  @param mac MACAddress of the new Host to create
   *  @param v4 IPv4Address of the new Host to create
   *  @param v6 IPv6Address of the new Host to create
   */
  Host(const MACAddress mac, const IPv4Address v4, const IPv6Address v6);

  /** 
   *  @brief Accessor method for the const_ property
   *  @retval MACAddress media access control address for this Host
   */
  MACAddress mac() const;

  /**
   *  @brief Accessor method for the ipv4_ property
   *  @retval IPv4Address Internet Protocol version 4 address for this Host
   */
  IPv4Address ipv4() const;

  /**
   *  @brief Accessor method for the ipv6_ property
   *  @retval IPv6Address Internet Protocol version 6 address for this Host
   */
  IPv6Address ipv6() const;

  /**
   *  @brief Mutator method for the ipv4_ property
   *  @param ipv4 Internet Protocol version 4 address for this Host
   */
  void set_ipv4(const IPv4Address ipv4);

  /**
   *  @brief Mutator method for the ipv6_ property
   *  @param ipv6 Internet Protocol version 6 address for this Host
   */
  void set_ipv6(const IPv6Address ipv6);
};

/**
 *  @brief Overload the < binary infix comparison operator
 *  @details IPForensics::hosts_ stores Hosts as an std::set so this class must
 *           overload the < operator so that the Hosts sort correctly
 */
bool operator<(const Host& lhs, const Host& rhs);

/**
 *  @brief Overload the > binary infix comparison operator
 */
bool operator>(const Host& lhs, const Host& rhs);

/**
 *  @brief Overload the == binary infix comparison operator
 */
bool operator==(const Host& lhs, const Host& rhs);

/**
 *  @brief Overload the != binary infix comparison operator
 */
bool operator!=(const Host& lhs, const Host& rhs);

/**
 *  @brief Overload the <= binary infix comparison operator
 */
bool operator<=(const Host& lhs, const Host& rhs);

/**
 *  @brief Overload the >= binary infix comparison operator
 */
bool operator>=(const Host& lhs, const Host& rhs);

/**
 *  @brief Provide the std::string representation of a Host by overloading the
 *         << operator for std::ostream
 *  @param out std::ostream output stream
 *  @param h Host instance to display as an std::string
 *  @retval std::ostream address that contains the std::string representation of
 *          this Host
 */
std::ostream& operator<<(std::ostream& out, const Host& h);

#endif  // IPFORENSICS_HOST_H_
