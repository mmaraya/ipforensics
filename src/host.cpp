//
// The MIT License (MIT)
//
// Copyright (c) 2014 Michael Maraya
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

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

bool Host::operator<(const Host &rhs) const {
  return (rhs.mac().str().compare(mac().str()) > 0);
}

MACAddress Host::mac() const {
  return mac_;
}

IPv4Address Host::ipv4() const {
  return ipv4_;
}

void Host::set_ipv4(const IPv4Address ipv4) {
  ipv4_ = ipv4;
}

IPv6Address Host::ipv6() const {
  return ipv6_;
}

void Host::set_ipv6(const IPv6Address ipv6) {
  ipv6_ = ipv6;
}

ostream &operator<<(ostream &out, const Host &h) {
  string mac = h.mac().str();
  string ipv4 = h.ipv4().str();
  string ipv6 = h.ipv6().str();
  out << left << setfill(' ');
  out << setw(18) << mac;
  out << setw(16) << ipv4;
  out << setw(39) << ipv6;
  return out;
}
