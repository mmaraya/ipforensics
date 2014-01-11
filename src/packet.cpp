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
    
    // extract IPv4 source and destination addresses
    if (etherType[0] == ETHERTYPE_IPV4[0] && etherType[1] == ETHERTYPE_IPV4[1]) {
        for (int i = PACKET_OFFFSET_IPV4_SRC; i < PACKET_OFFFSET_IPV4_SRC + IPV4_LENGTH; ++i) {
            srcV4[i - PACKET_OFFFSET_IPV4_SRC] = p[i];
        }
        for (int i = PACKET_OFFFSET_IPV4_DST; i < PACKET_OFFFSET_IPV4_DST + IPV4_LENGTH; ++i) {
            dstV4[i - PACKET_OFFFSET_IPV4_DST] = p[i];
        }
    }
    
    // extract IPv6 source and destination addresses
    if (etherType[0] == ETHERTYPE_IPV6[0] && etherType[1] == ETHERTYPE_IPV6[1]) {
        for (int i = PACKET_OFFFSET_IPV6_SRC; i < PACKET_OFFFSET_IPV6_SRC + IPV6_LENGTH; ++i) {
            srcV6[i - PACKET_OFFFSET_IPV6_SRC] = p[i];
        }
        for (int i = PACKET_OFFFSET_IPV6_DST; i < PACKET_OFFFSET_IPV6_DST + IPV6_LENGTH; ++i) {
            dstV6[i - PACKET_OFFFSET_IPV6_DST] = p[i];
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


