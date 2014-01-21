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

Packet::Packet(const unsigned char * p) {
  
  // extract the source MAC address from the packet
  std::vector<unsigned char> mac_src;
  for (int i = ipf::kOffsetMACSrc; i < ipf::kOffsetMACSrc + ipf::kLengthMAC; ++i) {
    mac_src.push_back(p[i]);
  }
  mac_src_.set_address(mac_src);
  
  // extract the destination MAC address from the packet
  std::vector<unsigned char> mac_dst;
  for (int i = ipf::kOffsetMACDst; i < ipf::kOffsetMACDst + ipf::kLengthMAC; ++i) {
    mac_dst.push_back(p[i]);
  }
  mac_dst_.set_address(mac_dst);
  
  // extract the ethernet type
  for (int i = ipf::kOffsetEtherType; i < ipf::kOffsetEtherType + ipf::kLengthEtherType; ++i) {
    ether_type_[i - ipf::kOffsetEtherType] = p[i];
  }
  
  if (ipv4()) {

    // extract IPv4 source addresses
    std::vector<unsigned char> ipv4_src;
    for (int i = ipf::kOffsetIPv4Src; i < ipf::kOffsetIPv4Src + ipf::kLengthIPv4; ++i) {
      ipv4_src.push_back(p[i]);
    }
    ipv4_src_.set_address(ipv4_src);
    
    // extract IPv4 destination addresses
    std::vector<unsigned char> ipv4_dst;
    for (int i = ipf::kOffsetIPv4Dst; i < ipf::kOffsetIPv4Dst + ipf::kLengthIPv4; ++i) {
      ipv4_dst.push_back(p[i]);
    }
    ipv4_dst_.set_address(ipv4_dst);
  }
  
  if (ipv6()) {

    // extract IPv6 source addresses
    std::vector<unsigned char> ipv6_src;
    for (int i = ipf::kOffsetIPv6Src; i < ipf::kOffsetIPv6Src + ipf::kLengthIPv6; ++i) {
      ipv6_src.push_back(p[i]);
    }
    ipv6_src_.set_address(ipv6_src);

    // extract IPv6 destination addresses
    std::vector<unsigned char> ipv6_dst;
    for (int i = ipf::kOffsetIPv6Dst; i < ipf::kOffsetIPv6Dst + ipf::kLengthIPv6; ++i) {
      ipv6_dst.push_back(p[i]);
    }
    ipv6_dst_.set_address(ipv6_dst);
  }
  
}

MACAddress Packet::mac_src() const {
  return mac_src_;
}

MACAddress Packet::mac_dst() const {
  return mac_dst_;
}

unsigned short * Packet::ether_type() {
  return ether_type_;
}

IPv4Address Packet::ipv4_src() const {
  return ipv4_src_;
}

IPv4Address Packet::ipv4_dst() const {
  return ipv4_dst_;
}

IPv6Address Packet::ipv6_src() const {
  return ipv6_src_;
}

IPv6Address Packet::ipv6_dst() const {
  return ipv6_dst_;
}

//
// return true if this packet contains IPv4
//
bool Packet::ipv4() const {
  return (ether_type_[0] == ipf::kEtherTypeIPv4[0] &&
          ether_type_[1] == ipf::kEtherTypeIPv4[1]);
}

//
// return true if this packet contains IPv6
//
bool Packet::ipv6() const {
  return (ether_type_[0] == ipf::kEtherTypeIPv6[0] &&
          ether_type_[1] == ipf::kEtherTypeIPv6[1]);
}

std::ostream &operator<<(std::ostream &out, const Packet &p) {
  out << p.mac_src() << " -> " << p.mac_dst() << ' ';
  if (p.ipv4()) {
    out << p.ipv4_src() << " -> " << p.ipv4_dst();
  }
  if (p.ipv6()) {
    out << p.ipv6_src() << " -> " << p.ipv6_dst();
  }
  return out;
}


