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

#include <iostream>
#include "host.h"

/**
 *  @brief Model class for storing information about a single network node,
 *         following the model-view-controller software design pattern
 *  @details Host stores the media access control, Internet Protocol version 4,
 *           and Internet Protocol version 6 address information for a single
 *           network node.
 */
class Packet {
private:
  MACAddress mac_src_;
  MACAddress mac_dst_;
  unsigned short ether_type_ {};
  IPv4Address ipv4_src_ {};
  IPv4Address ipv4_dst_ {};
  IPv6Address ipv6_src_ {};
  IPv6Address ipv6_dst_ {};
public:
  Packet(const unsigned char *);
  bool ipv4() const;
  bool ipv6() const;
  MACAddress mac_src() const;
  MACAddress mac_dst() const;
  unsigned short ether_type() const;
  IPv4Address ipv4_src() const;
  IPv4Address ipv4_dst() const;
  IPv6Address ipv6_src() const;
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

