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
 *  @details Address is the base class used by IPForensics to store MAC, IPv4 
 *           and IPv6 addresses.  Address uses a vector of unsigned char and 
 *           relies on descendant classes to set the elements of the vector and 
 *           provide the string representation.
 */
class Address {
 protected:

  /**
   *  @brief Internal representation of a network address
   *  @details Descendant classes are expected to set the value of the
   *           std::vector and an empty std::vector indicates that the address 
   *           has not yet been set.
   */
  std::vector<unsigned char> address_;

 public:

  /**
   *  @brief Descendant classes require the Address class to have a default 
   *         constructor
   */
  Address();

  /**
   *  @brief Creates an Address with the supplied std::vector of unsigned char
   *  @param address replaces the value of the internal property address_
   */
  Address(std::vector<unsigned char> address);

  /**
   *  @brief Compares the internal contents of this Address against another one
   *  @param b the other address to compare against
   *  @retval bool true if size and contents of address_ match, false otherwise
   */
  bool operator==(const Address& b) const;

  /**
   *  @brief Compares the internal contents of this Address against another one
   *  @param b the other address to compare against
   *  @retval bool false if size and contents of address_ match, true otherwise
   */
  bool operator!=(const Address& b) const;

  /**
   *  @brief Accessor for the address_ property
   *  @retval std::vector copy of the address_ property
   */
  std::vector<unsigned char> address() const;

  /**
   *  @brief Mutator for the address_ property
   *  @param address new value of the address_ property
   */
  void set_address(std::vector<unsigned char> address);

  /**
   *  @brief Pure virtual function for the human-readable representation of the
   *         address being stored.  Descendant classes are responsible for its
   *         implementation.
   *  @retval std::string representation of this address
   */
  virtual std::string str() const = 0 ;
};

/**
 *  @brief Provide the std::string representation of an Address by overloading
 *         the << operator for std::ostream
 *  @param out std::ostream output stream
 *  @param a Address instance to display as an std::string
 *  @retval std::ostream address that contains the std::string representation of 
 *          this Address
 */
std::ostream& operator<<(std::ostream& out, const Address& a);

/**
 *  @brief Media access control (MAC) address
 *  @details MACAddress is a descendant of the Address class and represents a
 *           6-octet hexadecimal media access control address. It should be 
 *           possible to look up vendor information using this property.
 */
class MACAddress : public Address {
 public:

  /**
   *  @brief Creates a new media access control address with an empty 
   *         Address::address_
   */
  MACAddress();

  /**
   *  @brief Creates a new media access control address with the supplied value
   *  @details This constructor invokes the Address ancestor class constructor
   *           of the same signature
   *  @param address is used to set the internal Address::address_ property
   */
  MACAddress(std::vector<unsigned char> address) : Address(address) {};

  /**
   *  @brief Provides a human-readable std::string representation of this media
   *         access control address
   *  @retval std::string representation of this MAC address
   */
  virtual std::string str() const override;
};

/**
 *  @brief Internet Protocol version 4 (IPv4) address
 *  @details IPv4Address is a descendant of the Address class and is used to
 *           store and display the four decimals that make up an IPv4 address 
 *           using the familiar quad-dotted notation.  This class can also be 
 *           used to store IPv4 network addresses, store network masks, and 
 *           determine if another IPv4Address is on the same subnet.
 */
class IPv4Address : public Address {
 public:
 
  /**
   *  @brief Creates a new IPv4 address with an empty Address::address_
   */
  IPv4Address();

  /**
   *  @brief Creates a new IPv4 address with the supplied unsigned 32-bit value
   *  @param address 32-bit value to be converted into Address::address_
   */
  IPv4Address(const unsigned int address);

  /**
   *  @brief Creates a new IPv4 address with the supplied std::vector
   *  @details This constructor invokes the Address ancestor class constructor
   *           of the same signature
   *  @param address is used to set the internal Address::address_ property
   */
  IPv4Address(std::vector<unsigned char> address) : Address(address) {};

  /**
   *  @brief Provides a human-readable std::string representation of this IPv4
   *         address
   *  @retval std::string representation of this IPv4 address
   */
  virtual std::string str() const override;

  /**
   *  @brief Tests if this IPv4Address is within the supplied subnet
   *  @param addr network address to test this IPv4Address against
   *  @param mask network mask to test this IPv4Address against
   *  @retval bool true if this IPv4Address is within the supplied network 
   *          address (addr) and network mask (mask), false otherwise
   */
  bool mask(IPv4Address addr, IPv4Address mask) const;
};

/**
 *  @brief Internet Protocol version 6 (IPv6) address
 *  @details IPv6Address is a descendant of the Address class and is used to 
 *           store 128-bit Internet Protocol version 6 addresses and display 
 *           them as eight colon-separated groups of four hexadecimal digits.
 */
class IPv6Address : public Address {
 public:

  /**
   *  @brief Creates a new IPv6 address with an empty Address::address_
   */
  IPv6Address();
  
  /**
   *  @brief Creates a new IPv6 address with the supplied std::vector
   *  @details This constructor invokes the Address ancestor class constructor
   *           of the same signature
   *  @param address is used to set the internal Address::address_ property
   */
  IPv6Address(std::vector<unsigned char> address) : Address(address) {};

  /**
   *  @brief Provides a human-readable std::string representation of this IPv6
   *         address
   *  @retval std::string representation of this IPv4 address
   */
  virtual std::string str() const override;
};

/**
 *  @brief IPForensics program-wide constants and utility functions
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
  const IPv4Address kBroadcastIPv4 {std::vector<unsigned char> (4, 0xFF)};
  
  /** MAC broadcast address */
  const MACAddress kBroadcastMAC {std::vector<unsigned char> (6, 0xFF)};
  
  /** output header for comma-separated values file */
  const std::string kHeaderCSV {"MAC Address,IPv4 Address,IPv6 Address"};

  /** output header for console display */
  const std::string kHeader {"MAC Address       IPv4 Address    IPv6 Address\n"
    + std::string(17,'=') + ' ' + std::string(15,'=') + ' '
    + std::string(39,'=')};
}
