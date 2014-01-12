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

#include <iostream>
#include "host.h"
using namespace std;

class Packet {
private:
  unsigned char mac_src_[ipf::kLengthMAC] {};
  unsigned char mac_dst_[ipf::kLengthMAC] {};
  unsigned short ether_type_[ipf::kLengthEtherType] {};
  unsigned char ipv4_src_[ipf::kLengthIPv4] {};
  unsigned char ipv4_dst_[ipf::kLengthIPv4] {};
  unsigned char ipv6_src_[ipf::kLengthIPv6] {};
  unsigned char ipv6_dst_[ipf::kLengthIPv6] {};
public:
  Packet(const unsigned char *);
  bool ipv4();
  bool ipv6();
  unsigned char * mac_src();
  unsigned char * mac_dst();
  unsigned short * ether_type();
  unsigned char * ipv4_src();
  unsigned char * ipv4_dst();
  unsigned char * ipv6_src();
  unsigned char * ipv6_dst();
};

