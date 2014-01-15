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

#include "packet.h"
using namespace ipf;

Packet::Packet(const unsigned char * p) {
  
  // extract the source MAC address from the packet
  vector<unsigned char> mac_src;
  for (int i = kOffsetMACSrc; i < kOffsetMACSrc + kLengthMAC; ++i) {
    mac_src.push_back(p[i]);
  }
  mac_src_.set_address(mac_src);
  
  // extract the destination MAC address from the packet
  vector<unsigned char> mac_dst;
  for (int i = kOffsetMACDst; i < kOffsetMACDst + kLengthMAC; ++i) {
    mac_dst.push_back(p[i]);
  }
  mac_dst_.set_address(mac_dst);
  
  // extract the ethernet type
  for (int i = kOffsetEtherType; i < kOffsetEtherType + kLengthEtherType; ++i) {
    ether_type_[i - kOffsetEtherType] = p[i];
  }
  
  if (ipv4()) {

    // extract IPv4 source addresses
    vector<unsigned char> ipv4_src;
    for (int i = kOffsetIPv4Src; i < kOffsetIPv4Src + kLengthIPv4; ++i) {
      ipv4_src.push_back(p[i]);
    }
    ipv4_src_.set_address(ipv4_src);
    
    // extract IPv4 destination addresses
    vector<unsigned char> ipv4_dst;
    for (int i = kOffsetIPv4Dst; i < kOffsetIPv4Dst + kLengthIPv4; ++i) {
      ipv4_dst.push_back(p[i]);
    }
    ipv4_dst_.set_address(ipv4_dst);
  }
  
  if (ipv6()) {

    // extract IPv6 source addresses
    vector<unsigned char> ipv6_src;
    for (int i = kOffsetIPv6Src; i < kOffsetIPv6Src + kLengthIPv6; ++i) {
      ipv6_src.push_back(p[i]);
    }
    ipv6_src_.set_address(ipv6_src);

    // extract IPv6 destination addresses
    vector<unsigned char> ipv6_dst;
    for (int i = kOffsetIPv6Dst; i < kOffsetIPv6Dst + kLengthIPv6; ++i) {
      ipv6_dst.push_back(p[i]);
    }
    ipv6_dst_.set_address(ipv6_dst);
  }
  
}

MACAddress Packet::mac_src() {
  return mac_src_;
}

MACAddress Packet::mac_dst() {
  return mac_dst_;
}

unsigned short * Packet::ether_type() {
  return ether_type_;
}

IPv4Address Packet::ipv4_src() {
  return ipv4_src_;
}

IPv4Address Packet::ipv4_dst() {
  return ipv4_dst_;
}

IPv6Address Packet::ipv6_src() {
  return ipv6_src_;
}

IPv6Address Packet::ipv6_dst() {
  return ipv6_dst_;
}

//
// return true if this packet contains IPv4
//
bool Packet::ipv4() {
  return (ether_type_[0] == kEtherTypeIPv4[0] &&
          ether_type_[1] == kEtherTypeIPv4[1]);
}

//
// return true if this packet contains IPv6
//
bool Packet::ipv6() {
  return (ether_type_[0] == kEtherTypeIPv6[0] &&
          ether_type_[1] == kEtherTypeIPv6[1]);
}


