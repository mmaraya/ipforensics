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

#ifndef IPFORENSICS_ADDRESS_H_
#define IPFORENSICS_ADDRESS_H_

#include <stdint.h>
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
  std::vector<uint8_t> address_;

 public:
  /**
   *  @brief Descendant classes require the Address class to have a default 
   *         constructor
   */
  Address();

  /**
   *  @brief Creates an Address from the supplied std::vector of unsigned char
   *  @param address replaces the value of the internal property address_
   */
  explicit Address(std::vector<uint8_t> address);

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
  std::vector<uint8_t> address() const;

  /**
   *  @brief Mutator for the address_ property
   *  @param address new value of the address_ property
   */
  void set_address(std::vector<uint8_t> address);

  /**
   *  @brief Pure virtual function for the human-readable representation of the
   *         address being stored.  Descendant classes are responsible for its
   *         implementation.
   *  @retval std::string representation of this address
   */
  virtual std::string str() const = 0;

  /**
   *  @brief Pure virtual function to check if this address is a broadcast, 
   *         multicast or otherwise useless address for network asset discovery
   *         purposes.
   *  @retval bool true if this is a useless address, false otherwise
   */
  virtual bool fake() const = 0;

  /**
   *  @brief Check if the Address has not been set
   *  @retval bool true if the address has not been set, false if set
   */
  bool empty() const;
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
 *  @brief Model class for Media access control (MAC) addresses, following the 
 *         model-view-controller software design pattern
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
   *  @brief Creates a new media access control address from the supplied
   *         std::string
   *  @param mac text representation of a MAC address
   */
  explicit MACAddress(const std::string mac);

  /**
   *  @brief Creates a new media access control address from the supplied value
   *  @details This constructor invokes the Address ancestor class constructor
   *           of the same signature
   *  @param address is used to set the internal Address::address_ property
   */
  explicit MACAddress(std::vector<uint8_t> address) : Address(address) {}

  /**
   *  @brief Provides a human-readable std::string representation of this media
   *         access control address
   *  @retval std::string representation of this MAC address
   */
  virtual std::string str() const override;

  /**
   *  @brief Check if this address is a broadcast, multicast or otherwise 
   *         useless address for network asset discovery purposes.
   *  @retval bool true if this is a useless address, false otherwise
   */
  virtual bool fake() const override;
};

/**
 *  @brief Model class for Internet Protocol version 4 (IPv4) addresses, 
 *         following the model-view-controller software design pattern
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
   *  @brief Creates a new IPv4 address from the supplied std::string
   *  @param ipv4 IPv4 address in dotted-quad notation
   */
  explicit IPv4Address(std::string ipv4);

  /**
   *  @brief Creates a new IPv4 address from the supplied unsigned 32-bit value
   *  @param address 32-bit value to be converted into Address::address_
   */
  explicit IPv4Address(const uint32_t address);

  /**
   *  @brief Creates a new IPv4 address from the supplied std::vector
   *  @details This constructor invokes the Address ancestor class constructor
   *           of the same signature
   *  @param address is used to set the internal Address::address_ property
   */
  explicit IPv4Address(std::vector<uint8_t> address) : Address(address) {}

  /**
   *  @brief Provides a human-readable std::string representation of this IPv4
   *         address
   *  @retval std::string representation of this IPv4 address
   */
  virtual std::string str() const override;

  /**
   *  @brief Check if this address is a broadcast, multicast or otherwise
   *         useless address for network asset discovery purposes.
   *  @retval bool true if this is a useless address, false otherwise
   */
  virtual bool fake() const override;

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
 *  @brief Model class for Internet Protocol version 6 (IPv6) addresses,
 *         following the model-view-controller software design pattern
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
   *  @brief Creates a new IPv6 address from the supplied std::string
   *  @param ipv6 IPv6 address in colon-separated notation
   */
  explicit IPv6Address(std::string ipv6);

  /**
   *  @brief Creates a new IPv6 address from the supplied std::vector
   *  @details This constructor invokes the Address ancestor class constructor
   *           of the same signature
   *  @param address is used to set the internal Address::address_ property
   */
  explicit IPv6Address(std::vector<uint8_t> address) : Address(address) {}

  /**
   *  @brief Provides a human-readable std::string representation of this IPv6
   *         address
   *  @retval std::string representation of this IPv4 address
   */
  virtual std::string str() const override;

  /**
   *  @brief Check if this address is a broadcast, multicast or otherwise
   *         useless address for network asset discovery purposes.
   *  @retval bool true if this is a useless address, false otherwise
   */
  virtual bool fake() const override;
};

#endif  // IPFORENSICS_ADDRESS_H_
