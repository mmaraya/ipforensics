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

/**
 *  @brief Base class for representing MAC, IPv4 and IPv6 addresses
 *
 *  Address is the base class used by IPForensics to store MAC, IPv4 and IPv6
 *  addresses.  Address uses a vector of unsigned char and relies on descendant 
 *  classes to set the size of the vector and provide the string representation.
 */
class Address {
 protected:
  /** 
   *  @brief Internal representation of a network address
   *  @details Descendant classes are expected to set the size and optionally
   *           the values of the std::vector during construction
   */
  std::vector<unsigned char> address_;
 public:
  Address();
  Address(std::vector<unsigned char> address);
  bool operator==(const Address&) const;
  bool operator!=(const Address&) const;
  std::vector<unsigned char> address() const;
  void set_address(std::vector<unsigned char>);
  virtual std::string str() const = 0 ;
};

/**
 *  @brief Provide the std::string representation of an Address by overloading
 *         the << operator for std::ostream
 *  
 *  @param out std::ostream output stream
 *  @param a Address instance to display as an std::string
 *  @retval pointer to std::ostream that contains the std::string representation
 *          of this Address
 */
std::ostream& operator<<(std::ostream& out, const Address& a);

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

  /**
   *  Tests if this IPv4Address is within the supplied network address and mask
   *
   *  @param addr network address to test this IPv4Address against
   *  @param mask network mask to test this IPv4Address against
   *  @retval true if this IPv4Address is within the supplied network address 
   *          (addr) and network mask (mask), false otherwise
   */
  bool mask(IPv4Address addr, IPv4Address mask);
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

/**
 *  IPForensics program-wide constants and utility functions
 */
namespace ipf {
  
  /** program name */
  const std::string kProgramName {"ipforensics"};
  
  /** program major revision number */
  const int kMajorVersion {0};
  
  /** program minor revision number */
  const int kMinorVersion {8};
  
  /** ethernet frame snapshot length */
  const int kSnapLength {256};
  
  /** number of milliseconds to wait for each network packet */
  const int kTimeout {1000};
  
  /** number of segments in a MAC address */
  const int kLengthMAC {6};
  
  /** number of segments in an IPv4 address */
  const int kLengthIPv4 {4};
  
  /** number of segments in an IPv6 address */
  const int kLengthIPv6 {16};
  
  /** MAC source address packet offset */
  const int kOffsetMACSrc {6};
  
  /** MAC destination address packet offset */
  const int kOffsetMACDst {0};
  
  /** ethertype packet offset */
  const int kOffsetEtherType {12};

  /** IPv4 source address packet offset */
  const int kOffsetIPv4Src {26};
  
  /** IPv4 destination address packet offset */
  const int kOffsetIPv4Dst {30};
  
  /** IPv6 source address packet offset */
  const int kOffsetIPv6Src {22};
  
  /** IPv6 destination address packet offset */
  const int kOffsetIPv6Dst {38};
  
  /** ethertype for IPv4 */
  const unsigned short kEtherTypeIPv4 {0x0800};
  
  /** ethertype for IPv6 */
  const unsigned short kEtherTypeIPv6 {0x86DD};
  
  /** IPv4 prefix for multicasts */
  const unsigned char kMulticastIPv4 {0xE};

  /** IPv4 broadcast address */
  const IPv4Address kBroadcastIPv4 {std::vector<unsigned char> (4, 255)};
  
  /** MAC broadcast address */
  const MACAddress kBroadcastMAC {std::vector<unsigned char> (6, 255)};
  
  /** output header for comma-separated values file */
  const std::string kHeaderCSV {"MAC Address,IPv4 Address,IPv6 Address"};

  /** output header for console display */
  const std::string kHeader {"MAC Address       IPv4 Address    IPv6 Address\n"
    + std::string(17,'=') + ' ' + std::string(15,'=') + ' '
    + std::string(39,'=')};
  
  /**
   *  @brief Convert array of unsigned short vlaues to hexadecimal
   *  @param array pointer to unsigned short values
   *  @param length number of elements in the array
   *  @retval std::string colon-separated groupings of two hexadecimal digits
   */
  std::string hexStr(const unsigned short* array, const int length);
}
