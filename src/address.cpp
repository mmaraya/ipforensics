/**
 *  @file address.cpp
 *  @brief Address class hierarchy implementation
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
#include <sstream>
#include <string>
#include <vector>
#include "ipforensics/ip4and6.h"
#include "ipforensics/address.h"

Address::Address() {
}

Address::Address(std::vector<uint8_t> address) {
  address_ = address;
}

std::vector<uint8_t> Address::address() const {
  return address_;
}

void Address::set_address(std::vector<uint8_t> address) {
  address_ = address;
}

bool Address::empty() const {
  return address_.empty();
}

std::ostream& operator<<(std::ostream& out, const Address& a) {
  return (out << a.str());
}

bool Address::operator==(const Address &b) const {
  if (address().size() != b.address().size()) return false;
  for (uint32_t i = 0; i < address().size(); ++i) {
    if (address()[i] != b.address()[i]) return false;
  }
  return true;
}

/** 
 *  @details The implementation of this method simply returns the opposite of
 *           the Address::operator== method to ensure consistency and reduce
 *           duplicative code.
 */
bool Address::operator!=(const Address &b) const {
  return !(*this == b);
}

MACAddress::MACAddress() {
}

MACAddress::MACAddress(const std::string mac) {
  for (size_t i = 0; i < ipf::kLengthMAC; i++) {
    std::string segment = mac.substr(i*3, 2);
    uint8_t value = static_cast<uint8_t>(std::stoi(segment, 0, 16));
    Address::address_.push_back(value);
  }
}

std::string MACAddress::str() const {
  std::stringstream ss;
  if (!address_.empty()) {
    for (size_t i = 0; i < ipf::kLengthMAC; ++i) {
      if (i > 0) ss << ':';
      ss << std::hex << std::setw(2) << std::setfill('0');
      ss << static_cast<int>(address_[i]);
    }
  }
  return ss.str();
}

bool MACAddress::fake() const {
  if (address_ == ipf::kBroadcastMAC.address()) {
    return true;
  }
  return false;
}

IPv4Address::IPv4Address() {
}

/**
 *  @details While libpcap and WinPcap both have utilities to convert strings
 *           to IP Addresses, we decided to limit the dependencies on these
 *           external libraries to capturing packets
 */
IPv4Address::IPv4Address(std::string ipv4) {
  size_t start = 0, end = ipv4.find('.');
  while (end != std::string::npos) {
    std::string segment = ipv4.substr(start, end - start);
    uint8_t value = static_cast<uint8_t>(std::stoi(segment, 0, 10));
    Address::address_.push_back(value);
    start = end + 1;
    if (end == ipv4.rfind('.')) {
      end = ipv4.length() - 1;
    } else {
      end = ipv4.find('.', start);
    }
  }
}

/**
 *  @details The implementation of this method shifts the supplied 32-bit
 *           unsigned int by 0, 8, 16, and 24 bits to isolate the octets and
 *           load them into the corresponding unsigned char vector element.
 */
IPv4Address::IPv4Address(const uint32_t address) {
  address_ = std::vector<uint8_t>(ipf::kLengthIPv4);
  for (size_t i = 0; i < ipf::kLengthIPv4; ++i) {
    address_[i] = static_cast<uint8_t>(address >> (8 * i));
  }
}

std::string IPv4Address::str() const {
  std::stringstream ss;
  if (!address_.empty()) {
    for (size_t i = 0; i < ipf::kLengthIPv4; ++i) {
      if (i > 0) ss << '.';
      ss << static_cast<int>(address_[i]);
    }
  }
  return ss.str();
}

bool IPv4Address::fake() const {
  if (!address_.empty()) {
    if (*this == ipf::kBroadcastIPv4) return true;
    uint8_t prefix = this->address()[0] >> 4;
    if ((prefix & ipf::kMulticastIPv4) == ipf::kMulticastIPv4) return true;
  }
  return false;
}

/**
 *  @details The implementation of this method takes the result of a bitwise
 *           AND operation between this IPv4Address and the network mask (mask)
 *           and compares that result against the network address (addr). If the
 *           two (the result and the network address) are the same, then we know
 *           that this IPv4Address is within the supplied subnet.
 */
bool IPv4Address::mask(IPv4Address addr, IPv4Address mask) const {
  IPv4Address subnet = IPv4Address(0);
  for (size_t i = 0; i < ipf::kLengthIPv4; ++i) {
    subnet.address_[i] = address_[i] & mask.address_[i];
  }
  return (subnet == addr);
}

IPv6Address::IPv6Address() {
}

IPv6Address::IPv6Address(const std::string ipv6) {
  std::string v6 = ipv6;
  if (v6.find(' ') != std::string::npos) {
    v6 = v6.substr(0, v6.find(' ') + 1);
  }
  size_t colon = static_cast<size_t>(std::count(v6.begin(), v6.end(), ':'));
  size_t zeroCompress = v6.find("::") + 1;
  size_t start = 0, end = v6.find(':');
  while (end != std::string::npos) {
    if (start == zeroCompress) {
      for (size_t i = 0 ; i < ipf::kLengthIPv6 - colon; i = i + 2) {
        Address::address_.push_back(0);
      }
      start = zeroCompress + 1;
      end = v6.find(':', start);
      if (end == std::string::npos) break;
    }
    std::string segment = "0x" + v6.substr(start, end - start);
    uint16_t val = static_cast<uint16_t>(std::stoul(segment, nullptr, 16));
    Address::address_.push_back(static_cast<uint8_t>(val >> 8));
    Address::address_.push_back(static_cast<uint8_t>(val & 0x00FF));
    start = end + 1;
    if (end == v6.rfind(':')) {
      end = v6.length() - 1;
    } else {
      end = v6.find(':', start + 1);
    }
  }
}

std::string IPv6Address::str() const {
  std::string result {};
  if (!address_.empty()) {
    std::vector<uint16_t> ipv6 {};
    for (size_t i = 0; i < ipf::kLengthIPv6; i+=2) {
      ipv6.push_back(static_cast<uint16_t>(address_[i] << 8 | address_[i+1]));
    }
    std::stringstream ss;
    ss << std::hex;
    for (size_t i = 0; i < ipv6.size(); ++i) {
      if (i > 0) ss << ':';
      ss << ipv6[i];
    }
    // compress longest series of 0:0 to :: (zero compression)
    result = ss.str();
    for (size_t i = ipv6.size() - 1; i > 1; --i) {
      std::string zero {};
      for (size_t j = 1; j < i + 1; ++j) {
        zero.append(":0");
      }
      size_t pos = ss.str().find(zero);
      if (pos != std::string::npos) {
        result.replace(pos, i * 2 + 1, "::");
        break;
      }
    }
  }
  return result;
}

bool IPv6Address::fake() const {
  if (!address_.empty() && address_[0] == 0xFF) {
    return true;
  }
  return false;
}
