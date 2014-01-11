//
// The MIT License (MIT)
//
// Copyright (c) 2014 Michael Maraya
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <iomanip>
#include <iostream>
#include <sstream>
#include "host.h"
using namespace std;

namespace ipf {
    const int kLengthMAC        {6};
    const int kLengthEtherType  {2};
    const int kLengthIPv4       {4};
    const int kLengthIPv6       {16};
    const int kOffsetMACSrc     {0};
    const int kOffsetMACDst     {6};
    const int kOffsetEtherType  {12};
    const int kOffsetIPv4Src    {26};
    const int kOffsetIPv4Dst    {30};
    const int kOffsetIPv6Src    {22};
    const int kOffsetIPv6Dst    {38};
    const unsigned char kEtherTypeIPv4[kLengthEtherType] {0x08, 0x00};
    const unsigned char kEtherTypeIPv6[kLengthEtherType] {0x86, 0xDD};
}

class Packet {
private:
    unsigned char srcMac[ipf::kLengthMAC] {};
    unsigned char dstMac[ipf::kLengthMAC] {};
    unsigned short etherType[ipf::kLengthEtherType] {};
    unsigned char srcV4[ipf::kLengthIPv4] {};
    unsigned char dstV4[ipf::kLengthIPv4] {};
    unsigned char srcV6[ipf::kLengthIPv6] {};
    unsigned char dstV6[ipf::kLengthIPv6] {};
public:
    Packet(const unsigned char *);
    bool ipv4();
    bool ipv6();
    unsigned char * getSrcMac();
    unsigned char * getDstMac();
    unsigned short * getType();
    unsigned char * getSrcV4();
    unsigned char * getDstV4();
    unsigned char * getSrcV6();
    unsigned char * getDstV6();
};

