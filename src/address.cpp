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

#include "ipforensics/ip4and6.h"
#include "ipforensics/address.h"

Address::Address() {
}

Address::Address(std::vector<unsigned char> address) {
  address_ = address;
}

std::vector<unsigned char> Address::address() const {
  return address_;
}

void Address::set_address(std::vector<unsigned char> address) {
  address_ = address;
}

std::ostream& operator<<(std::ostream& out, const Address& a) {
  return (out << a.str());
}

bool Address::operator==(const Address &b) const {
  if (address().size() != b.address().size()) return false;
  for (unsigned int i = 0; i < address().size(); ++i) {
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

std::string MACAddress::str() const {
  std::stringstream ss;
  if (!address_.empty()) {
    for (int i = 0; i < ipf::kLengthMAC; ++i) {
      if (i > 0) ss << ':';
      ss << std::hex << std::setw(2) << std::setfill('0');
      ss << int((unsigned char)address_[i]);
    }
  }
  return ss.str();
}

IPv4Address::IPv4Address() {
}

/**
 *  @details The implementation of this method shifts the supplied 32-bit
 *           unsigned int by 0, 8, 16, and 24 bits to isolate the octets and
 *           load them into the corresponding unsigned char vector element.
 */
IPv4Address::IPv4Address(const unsigned int address) {
  address_ = std::vector<unsigned char>(ipf::kLengthIPv4);
  for (int i = 0; i < ipf::kLengthIPv4; ++i) {
    address_[i] = address >> (8 * i);
  }
}

std::string IPv4Address::str() const {
  std::stringstream ss;
  if (!address_.empty()) {
    for (int i = 0; i < ipf::kLengthIPv4; ++i) {
      if (i > 0) ss << '.';
      ss << int((unsigned char)address_[i]);
    }
  }
  return ss.str();
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
  for (int i = 0; i < ipf::kLengthIPv4; ++i) {
    subnet.address_[i] = address_[i] & mask.address_[i];
  }
  return (subnet == addr);
}

IPv6Address::IPv6Address() {
}

/**
 *  @details Limited support is currently provided for shortening the display 
 *           of IPv6 addresses as specified in RFC 5952
 *  @todo Fully implement RFC 5952 (http://tools.ietf.org/html/rfc5952)
 */
std::string IPv6Address::str() const {
  std::stringstream ss;
  ss << std::hex;
  if (!address_.empty()) {
    for (int i = 0; i < ipf::kLengthIPv6; i+=2) {
      if (i > 0) ss << ':';
      ss << (unsigned short)(address_[i] << 8 | address_[i+1]);
    }
  }
  return ss.str();
}
