/**
 *  @file packet.cpp
 *  @brief Packet class implementation
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
#include <vector>
#include "ipforensics/ip4and6.h"
#include "ipforensics/packet.h"

/**
 *  @details IPv4, IPv6 and ARP are currently supported.
 */
Packet::Packet(const uint8_t * p) {
  // extract the source MAC address from the packet
  std::vector<uint8_t> mac_src;
  for (int i = ipf::kOffsetMACSrc; i < ipf::kOffsetMACSrc + ipf::kLengthMAC;
       ++i) {
    mac_src.push_back(p[i]);
  }
  mac_src_.set_address(mac_src);
  // extract the destination MAC address from the packet
  std::vector<uint8_t> mac_dst;
  for (int i = ipf::kOffsetMACDst; i < ipf::kOffsetMACDst + ipf::kLengthMAC;
       ++i) {
    mac_dst.push_back(p[i]);
  }
  mac_dst_.set_address(mac_dst);
  // extract the ethernet type
  ether_type_ = static_cast<uint16_t>((p[ipf::kOffsetEtherType] << 8) |
                                      p[ipf::kOffsetEtherType+1]);
  switch (ether_type_) {
    // Internet Protocol version 4 (ethertype 0800)
    case ipf::kEtherTypeIPv4: {
      std::vector<uint8_t> ipv4_src;
      for (int i = ipf::kOffsetIPv4Src;
           i < ipf::kOffsetIPv4Src + ipf::kLengthIPv4; ++i) {
        ipv4_src.push_back(p[i]);
      }
      ipv4_src_.set_address(ipv4_src);
      std::vector<uint8_t> ipv4_dst;
      for (int i = ipf::kOffsetIPv4Dst;
           i < ipf::kOffsetIPv4Dst + ipf::kLengthIPv4; ++i) {
        ipv4_dst.push_back(p[i]);
      }
      ipv4_dst_.set_address(ipv4_dst);
      break;
    }
    // Address Resolution Protocol (ethertype 0806)
    case ipf::kEtherTypeARP: {
      std::vector<uint8_t> ipv4_src;
      for (int i = ipf::kOffsetARPIPv4;
           i < ipf::kOffsetARPIPv4 + ipf::kLengthIPv4; ++i) {
        ipv4_src.push_back(p[i]);
      }
      ipv4_src_.set_address(ipv4_src);
      break;
    }
    // Internet Protocol version 6 (ethertype 08DD)
    case ipf::kEtherTypeIPv6: {
      std::vector<uint8_t> ipv6_src;
      for (int i = ipf::kOffsetIPv6Src;
           i < ipf::kOffsetIPv6Src + ipf::kLengthIPv6; ++i) {
        ipv6_src.push_back(p[i]);
      }
      ipv6_src_.set_address(ipv6_src);
      std::vector<uint8_t> ipv6_dst;
      for (int i = ipf::kOffsetIPv6Dst;
           i < ipf::kOffsetIPv6Dst + ipf::kLengthIPv6; ++i) {
        ipv6_dst.push_back(p[i]);
      }
      ipv6_dst_.set_address(ipv6_dst);
      break;
    }
  }
}

MACAddress Packet::mac_src() const { return mac_src_; }

MACAddress Packet::mac_dst() const { return mac_dst_; }

uint16_t Packet::ether_type() const { return ether_type_; }

IPv4Address Packet::ipv4_src() const { return ipv4_src_; }

IPv4Address Packet::ipv4_dst() const { return ipv4_dst_; }

IPv6Address Packet::ipv6_src() const { return ipv6_src_; }

IPv6Address Packet::ipv6_dst() const { return ipv6_dst_; }

bool Packet::ipv4() const { return (ether_type_ == ipf::kEtherTypeIPv4); }

bool Packet::ipv6() const { return (ether_type_ == ipf::kEtherTypeIPv6); }

std::ostream &operator<<(std::ostream &out, const Packet &p) {
  std::ios::fmtflags fmt(std::cout.flags());
  out << p.mac_src() << " -> " << p.mac_dst() << ' ';
  out << std::hex << std::setw(4) << std::setfill('0') << p.ether_type() << ' ';
  switch (p.ether_type()) {
    case ipf::kEtherTypeIPv4:
      out << p.ipv4_src() << " -> " << p.ipv4_dst();
      break;
    case ipf::kEtherTypeARP:
      out << p.ipv4_src();
      break;
    case ipf::kEtherTypeIPv6:
      out << p.ipv6_src() << " -> " << p.ipv6_dst();
      break;
  }
  std::cout.flags(fmt);
  return out;
}
