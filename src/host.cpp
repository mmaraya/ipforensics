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

#include "host.h"

//
// return the hex version of the unsigned char *
// TODO: Format IPv6 addresses using IETF RFC 5952
//
string ipf::hexStr(unsigned char * p, int len) {
  stringstream ss;
  for (int i = 0; i < len; ++i) {
    if (i > 0) ss << ':';
    ss << hex << setw(2) << setfill('0') << (int)(unsigned char)p[i];
  }
  return ss.str();
}

//
// return the hex version of the unsigned short
//
string ipf::hexStr(unsigned short * p, int len) {
  stringstream ss;
  for (int i = 0; i < len; ++i) {
    if (i > 0) ss << ':';
    ss << hex << setw(2) << setfill('0') << (int)(unsigned short)p[i];
  }
  return ss.str();
}

//
// return the integer version of the unsigned char *
//
string ipf::intStr(unsigned char * p, int len) {
  stringstream ss;
  for (int i = 0; i < len; ++i) {
    if (i > 0) ss << '.';
    ss << (int)(unsigned char)p[i];
  }
  return ss.str();
}

Host::Host(const string mac) {
  mac_ = mac;
}

Host::Host(const string mac, const string ipv4, const string ipv6) {
  mac_ = mac;
  ipv4_ = ipv4;
  ipv6_ = ipv6;
}

bool Host::operator<(const Host &rhs) const {
  return (this->mac().compare(rhs.mac()) > 0);
}

string Host::mac() const {
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
  out << h.mac() << ' ' << h.ipv4() << ' ' << h.ipv6();
  return out;
}
