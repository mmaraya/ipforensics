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

Host::Host(const MACAddress mac, const string ipv4, const string ipv6) {
  mac_ = mac;
  ipv4_ = ipv4;
  ipv6_ = ipv6;
}

bool Host::operator<(const Host &rhs) const {
  return (this->mac().str().compare(rhs.mac().str()) > 0);
}

MACAddress Host::mac() const {
  return mac_;
}

string Host::ipv4() const {
  return ipv4_;
}

void Host::set_ipv4(const string ipv4) {
  ipv4_ = ipv4;
}

string Host::ipv6() const {
  return ipv6_;
}

void Host::set_ipv6(const string ipv6) {
  ipv6_ = ipv6;
}

ostream &operator<<(ostream &out, const Host &h) {
  out << h.mac().str() << ' ' << h.ipv4() << ' ' << h.ipv6();
  return out;
}
