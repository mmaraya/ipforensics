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

static const int MAC_LENGTH = 6;
static const int ETHERTYPE_LENGTH = 2;
static const int IPV4_LENGTH = 4;
static const int IPV6_LENGTH = 16;

class Packet {
private:
    unsigned char srcMac[MAC_LENGTH] {};
    unsigned char dstMac[MAC_LENGTH] {};
    unsigned short etherType[ETHERTYPE_LENGTH] {};
    unsigned char addr4[IPV4_LENGTH] {};
    unsigned char addr6[IPV6_LENGTH] {};
    string hexStr(unsigned char *, int);
    string hexStr(unsigned short *, int);
public:
    Packet(const unsigned char *);
};