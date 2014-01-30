/** 
 *  @file address.h
 *  @brief Address class hierarchy definitions
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
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace ipf {
  const std::string kProgramName {"ipforensics"};
  const int kMajorVersion     {0};
  const int kMinorVersion     {8};
  const int kSnapLength       {256};
  const int kTimeout          {1000};
  const int kLengthMAC        {6};
  const int kLengthIPv4       {4};
  const int kLengthIPv6       {16};
  const int kOffsetMACSrc     {6};
  const int kOffsetMACDst     {0};
  const int kOffsetEtherType  {12};
  const int kOffsetIPv4Src    {26};
  const int kOffsetIPv4Dst    {30};
  const int kOffsetIPv6Src    {22};
  const int kOffsetIPv6Dst    {38};
  const unsigned short kEtherTypeIPv4 {0x0800};
  const unsigned short kEtherTypeIPv6 {0x86DD};
  const unsigned char kMulticastIPv4 {0xE};
  const std::string kBroadcastIPv4 {"255.255.255.255"};
  const std::string kBroadcastMAC {"ff:ff:ff:ff:ff:ff"};
  const std::string kCSVHeader {"MAC Address,IPv4 Address,IPv6 Address"};
  const std::string kNormalHeader {"MAC Address       IPv4 Address    IPv6 Address\n"
    + std::string(17,'=') + ' ' + std::string(15,'=') + ' ' + std::string(39,'=')};
  std::string hexStr(const unsigned short*, const int);
}

/**
 *  @brief Base class for representing MAC, IPv4 and IPv6 addresses
 *
 *  Address is the base class used by IPForensics to store MAC, IPv4 and IPv6
 *  addresses.  Address uses a vector of unsigned char and relies on descendant 
 *  classes to set the size of the vector and provide the string representation.
 */
class Address {
 protected:
  std::vector<unsigned char> address_;
 public:
  Address();
  Address(std::vector<unsigned char> address);
  bool operator==(const Address &) const;
  bool operator!=(const Address &) const;
  std::vector<unsigned char> address() const;
  void set_address(std::vector<unsigned char>);
  virtual std::string str() const = 0 ;
};

/**
 *  @brief Media access control (MAC) address
 *
 *  MACAddress is a descendant of the Address class and represents a 6-octet
 *  media access control address.  MAC addresses are represented as strings as
 *  six colon-separated hexadecimal segments.  Vendor information may be 
 *  extracted from the MAC address of a node in future versions of this program.
 */
class MACAddress : public Address {
 public:
  MACAddress();
  MACAddress(std::vector<unsigned char> address) : Address(address) {};
  virtual std::string str() const override;
};

/**
 *  @brief Internet Protocol version 4 (IPv4) address
 *
 *  IPv4Address is a descendant of the Address class and is used to store and
 *  display the four decimals that make up an IPv4 address using the familiar 
 *  quad-dotted notation.  This class can also be used to store IPv4 network
 *  addresses, store network masks, and determine if another IPv4Address is
 *  on the same subnet.
 */
class IPv4Address : public Address {
 public:
  IPv4Address();
  IPv4Address(const unsigned int);
  IPv4Address(std::vector<unsigned char> address) : Address(address) {};
  virtual std::string str() const override;
  bool mask(IPv4Address, IPv4Address);
};

/**
 *  @brief Internet Protocol version 6 (IPv6) address
 *
 *  IPv6Address is a descendant of the Address class and is used to store
 *  128-bit Internet Protocol version 6 addresses and display them as eight
 *  colon-separated groups of four hexadecimal digits.  Limited notation
 *  abbreviation capabilities are provided in this version.
 */
class IPv6Address : public Address {
public:
  IPv6Address();
  IPv6Address(std::vector<unsigned char> address) : Address(address) {};
  virtual std::string str() const override;
};

std::ostream &operator<<(std::ostream &out, const Address &a);
