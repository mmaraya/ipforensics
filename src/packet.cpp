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
    
    cout << hexStr(srcMac, MAC_LENGTH) << " -> " << hexStr(dstMac, MAC_LENGTH);
    cout << ' ' << hexStr(etherType, ETHERTYPE_LENGTH) << endl;

    // extract IPv4 address
    if (etherType[0] == ETHERTYPE_IPV4[0] && etherType[1] == ETHERTYPE_IPV4[1]) {
        for (int i = PACKET_OFFFSET_IPV4; i < PACKET_OFFFSET_IPV4 + IPV4_LENGTH; ++i) {
            ipv4[i - PACKET_OFFFSET_IPV4] = p[i];
        }
        cout << "IPv4 Address: " << intStr(ipv4, IPV4_LENGTH) << endl;
    }
    
    // extract IPv6 address
    if (etherType[0] == ETHERTYPE_IPV6[0] && etherType[1] == ETHERTYPE_IPV6[1]) {
        for (int i = PACKET_OFFFSET_IPV6; i < PACKET_OFFFSET_IPV6 + IPV6_LENGTH; ++i) {
            ipv6[i - PACKET_OFFFSET_IPV6] = p[i];
        }
        cout << "IPv6 Address: " << hexStr(ipv6, IPV6_LENGTH) << endl;
    }
    
}

//
// return the hex version of the unsigned char *
//
string Packet::hexStr(unsigned char * p, int len) {
    stringstream ss;
    for (int i = 0; i < len; ++i) {
        if (i > 0) ss << ':';
        ss << uppercase << hex << setw(2) << setfill('0') << (int)(unsigned char)p[i];
    }
    return ss.str();
}

//
// return the hex version of the unsigned short
//
string Packet::hexStr(unsigned short * p, int len) {
    stringstream ss;
    for (int i = 0; i < len; ++i) {
        if (i > 0) ss << ':';
        ss << uppercase << hex << setw(2) << setfill('0') << (int)(unsigned short)p[i];
    }
    return ss.str();
}

//
// return the integer version of the unsigned char *
//
string Packet::intStr(unsigned char * p, int len) {
    stringstream ss;
    for (int i = 0; i < len; ++i) {
        if (i > 0) ss << '.';
        ss << (int)(unsigned char)p[i];
    }
    return ss.str();
}