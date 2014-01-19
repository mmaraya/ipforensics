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

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

namespace ipf {
  const string kProgramName   {"ipforensics"};
  const int kMajorVersion     {0};
  const int kMinorVersion     {5};
  const int kSnapLength       {256};
  const int kTimeout          {1000};
  const int kLengthMAC        {6};
  const int kLengthEtherType  {2};
  const int kLengthIPv4       {4};
  const int kLengthIPv6       {16};
  const int kOffsetMACSrc     {6};
  const int kOffsetMACDst     {0};
  const int kOffsetEtherType  {12};
  const int kOffsetIPv4Src    {26};
  const int kOffsetIPv4Dst    {30};
  const int kOffsetIPv6Src    {22};
  const int kOffsetIPv6Dst    {38};
  const unsigned char kEtherTypeIPv4[kLengthEtherType] {0x08, 0x00};
  const unsigned char kEtherTypeIPv6[kLengthEtherType] {0x86, 0xDD};
  const string kCSVHeader {"MAC Address,IPv4 Address,IPv6 Address"};
  const string kNormalHeader {"MAC Address       IPv4 Address    IPv6 Address\n"
    + string(17,'=') + ' ' + string(15,'=') + ' ' + string(39,'=')};
  string hexStr(const unsigned short*, const int);
}

class Address {
 private:
 protected:
  vector<unsigned char> address_;
 public:
  Address();
  Address(vector<unsigned char> address);
  vector<unsigned char> address() const;
  void set_address(vector<unsigned char>);
  virtual string str() const = 0 ;
};

class MACAddress : public Address {
 private:
 public:
  MACAddress();
  MACAddress(vector<unsigned char> address) : Address(address) {};
  virtual string str() const override;
};

class IPv4Address : public Address {
 private:
 public:
  IPv4Address();
  IPv4Address(const unsigned int);
  IPv4Address(vector<unsigned char> address) : Address(address) {};
  virtual string str() const override;
  bool mask(IPv4Address, IPv4Address);
};

class IPv6Address : public Address {
private:
public:
  IPv6Address();
  IPv6Address(vector<unsigned char> address) : Address(address) {};
  virtual string str() const override;
};

bool operator==(const Address &a, const Address &b);

ostream &operator<<(ostream &out, const Address &a);
