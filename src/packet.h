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

static const int PACKET_OFFSET_SRC_MAC {0};
static const int PACKET_OFFSET_DST_MAC {6};
static const int MAC_LENGTH {6};
static const int PACKET_OFFSET_ETHERTYPE {12};
static const int ETHERTYPE_LENGTH {2};
static const int PACKET_OFFFSET_IPV4_SRC {26};
static const int PACKET_OFFFSET_IPV4_DST {30};
static const int IPV4_LENGTH {4};
static const int PACKET_OFFFSET_IPV6_SRC {22};
static const int PACKET_OFFFSET_IPV6_DST {38};
static const int IPV6_LENGTH {16};
static const unsigned char ETHERTYPE_IPV4[ETHERTYPE_LENGTH] {0x08, 0x00};
static const unsigned char ETHERTYPE_IPV6[ETHERTYPE_LENGTH] {0x86, 0xDD};

class Packet {
private:
    unsigned char srcMac[MAC_LENGTH] {};
    unsigned char dstMac[MAC_LENGTH] {};
    unsigned short etherType[ETHERTYPE_LENGTH] {};
    unsigned char srcV4[IPV4_LENGTH] {};
    unsigned char dstV4[IPV4_LENGTH] {};
    unsigned char srcV6[IPV6_LENGTH] {};
    unsigned char dstV6[IPV6_LENGTH] {};
public:
    Packet(const unsigned char *);
    unsigned char * getSrcMac();
    unsigned char * getDstMac();
    unsigned short * getType();
    unsigned char * getSrcV4();
    unsigned char * getDstV4();
    unsigned char * getSrcV6();
    unsigned char * getDstV6();
};

