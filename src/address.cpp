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

#include "address.h"

vector<unsigned char> Address::address() {
  return address_;
}

void Address::set_address(vector<unsigned char> address) {
  address_ = address;
}

MACAddress::MACAddress() {
  address_ = vector<unsigned char> (ipf::kLengthMAC);
}

MACAddress::MACAddress(vector<unsigned char> address) {
  address_ = address;
}

string MACAddress::str() {
  stringstream ss;
  for (int i = 0; i < ipf::kLengthMAC; ++i) {
    if (i > 0) ss << ':';
    ss << hex << setw(2) << setfill('0') << (int)(unsigned char)address_[i];
  }
  return ss.str();
}

//
// return the hex version of the unsigned char *
// TODO: Format IPv6 addresses using IETF RFC 5952
//
string ipf::hexStr(const unsigned char * p, const int len) {
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
string ipf::hexStr(const unsigned short * p, const int len) {
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
string ipf::intStr(const unsigned char * p, const int len) {
  stringstream ss;
  for (int i = 0; i < len; ++i) {
    if (i > 0) ss << '.';
    ss << (int)(unsigned char)p[i];
  }
  return ss.str();
}

