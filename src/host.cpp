/**
 *  @file host.cpp
 *  @brief Host class implementation
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

#include <iomanip>
#include "host.h"

Host::Host(const MACAddress mac) {
  mac_ = mac;
}

Host::Host(const MACAddress mac, const IPv4Address v4, const IPv6Address v6) {
  mac_ = mac;
  ipv4_ = v4;
  ipv6_ = v6;
}

MACAddress Host::mac() const {
  return mac_;
}

IPv4Address Host::ipv4() const {
  return ipv4_;
}

IPv6Address Host::ipv6() const {
  return ipv6_;
}

void Host::set_ipv4(const IPv4Address ipv4) {
  ipv4_ = ipv4;
}

void Host::set_ipv6(const IPv6Address ipv6) {
  ipv6_ = ipv6;
}

/**
 *  @details Uses the std::string.compare() function to compare the characters
 *           of the MAC address individually from left to right
 */
bool operator<(const Host& lhs, const Host& rhs) {
  return (rhs.mac().str().compare(lhs.mac().str()) > 0);
}

/**
 *  @details This method returns the command-line formatted representation of a
 *           Host and displays the MAC, IPv4 and IPv6 address padded by spaces
 *           so they align with the column headers in ipf::kHeader.
 */
std::ostream& operator<<(std::ostream& out, const Host& h) {
  out << std::left << std::setfill(' ');
  out << std::setw(18) << h.mac();
  out << std::setw(16) << h.ipv4();
  out << std::setw(39) << h.ipv6();
  return out;
}
