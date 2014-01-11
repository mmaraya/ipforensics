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

Packet::Packet(const unsigned char * p) {

    // extract the source MAC address from the packet
    for (int i = PACKET_OFFSET_SRC_MAC; i < PACKET_OFFSET_SRC_MAC + MAC_LENGTH; ++i) {
        srcMac[i - PACKET_OFFSET_SRC_MAC] = p[i];
    }
    
    // extract the destination MAC address from the packet
    for (int i = PACKET_OFFSET_DST_MAC; i < PACKET_OFFSET_DST_MAC + MAC_LENGTH; ++i) {
        dstMac[i - PACKET_OFFSET_DST_MAC] = p[i];
    }
    
    // extract the ethernet type
    for (int i = PACKET_OFFSET_ETHERTYPE; i < PACKET_OFFSET_ETHERTYPE + ETHERTYPE_LENGTH; ++i) {
        etherType[i - PACKET_OFFSET_ETHERTYPE] = p[i];
    }
    
    // extract IPv4 address
    if (etherType[0] == ETHERTYPE_IPV4[0] && etherType[1] == ETHERTYPE_IPV4[1]) {
        for (int i = PACKET_OFFFSET_IPV4; i < PACKET_OFFFSET_IPV4 + IPV4_LENGTH; ++i) {
            ipv4[i - PACKET_OFFFSET_IPV4] = p[i];
        }
    }
    
    // extract IPv6 address
    if (etherType[0] == ETHERTYPE_IPV6[0] && etherType[1] == ETHERTYPE_IPV6[1]) {
        for (int i = PACKET_OFFFSET_IPV6; i < PACKET_OFFFSET_IPV6 + IPV6_LENGTH; ++i) {
            ipv6[i - PACKET_OFFFSET_IPV6] = p[i];
        }
    }
    
}

unsigned char * Packet::src() {
    return srcMac;
}

unsigned char * Packet::dst() {
    return dstMac;
}

unsigned short * Packet::type() {
    return etherType;
}

unsigned char * Packet::v4() {
    return ipv4;
}

unsigned char * Packet::v6() {
    return ipv6;
}


