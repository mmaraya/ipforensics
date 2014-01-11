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

#include "packet.h"
using namespace ipf;

Packet::Packet(const unsigned char * p) {

    // extract the source MAC address from the packet
    for (int i = kOffsetMACSrc; i < kOffsetMACSrc + kLengthMAC; ++i) {
        srcMac[i - kOffsetMACSrc] = p[i];
    }
    
    // extract the destination MAC address from the packet
    for (int i = kOffsetMACDst; i < kOffsetMACDst + kLengthMAC; ++i) {
        dstMac[i - kOffsetMACDst] = p[i];
    }
    
    // extract the ethernet type
    for (int i = kOffsetEtherType; i < kOffsetEtherType + kLengthEtherType; ++i) {
        etherType[i - kOffsetEtherType] = p[i];
    }
    
    // extract IPv4 source and destination addresses
    if (etherType[0] == kEtherTypeIPv4[0] && etherType[1] == kEtherTypeIPv4[1]) {
        for (int i = kOffsetIPv4Src; i < kOffsetIPv4Src + kLengthIPv4; ++i) {
            srcV4[i - kOffsetIPv4Src] = p[i];
        }
        for (int i = kOffsetIPv4Dst; i < kOffsetIPv4Dst + kLengthIPv4; ++i) {
            dstV4[i - kOffsetIPv4Dst] = p[i];
        }
    }
    
    // extract IPv6 source and destination addresses
    if (etherType[0] == kEtherTypeIPv6[0] && etherType[1] == kEtherTypeIPv6[1]) {
        for (int i = kOffsetIPv6Src; i < kOffsetIPv6Src + kLengthIPv6; ++i) {
            srcV6[i - kOffsetIPv6Src] = p[i];
        }
        for (int i = kOffsetIPv6Dst; i < kOffsetIPv6Dst + kLengthIPv6; ++i) {
            dstV6[i - kOffsetIPv6Dst] = p[i];
        }
    }
    
}

unsigned char * Packet::getSrcMac() {
    return srcMac;
}

unsigned char * Packet::getDstMac() {
    return dstMac;
}

unsigned short * Packet::getType() {
    return etherType;
}

unsigned char * Packet::getSrcV4() {
    return srcV4;
}

unsigned char * Packet::getDstV4() {
    return dstV4;
}

unsigned char * Packet::getSrcV6() {
    return srcV6;
}

unsigned char * Packet::getDstV6() {
    return dstV6;
}


