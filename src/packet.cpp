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
    int i = 0;
    for (; i < MAC_LENGTH; ++i) {
        srcMac[i] = p[i];
    }
    
    // extract the destination MAC address from the packet
    for (int j = 0; j < MAC_LENGTH; ++j, ++i) {
        dstMac[j] = p[i];
    }

    cout << hexStr(srcMac, MAC_LENGTH) << " -> " << hexStr(dstMac, MAC_LENGTH) << endl;
}

//
// return the string version of the hexadecimal of len length
//
string Packet::hexStr(unsigned char * p, int len) {
    stringstream ss;
    for (int i = 0; i < len; ++i) {
        if (i > 0) ss << ':';
        ss << uppercase << hex << setw(2) << setfill('0') << (int)(unsigned char)p[i];
    }
    return ss.str();
}