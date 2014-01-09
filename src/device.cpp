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

#include "device.h"

Device::Device() {}

Device::Device(string n, string d="", bool l=false) {
    name = n;
    description = d;
    loopback = l;
}

const string Device::getName() const {
    return name;
}

void Device::setName(string str) {
    name = str;
}

const string Device::getDescription() const {
    return description;
}

void Device::setDescription(string str) {
    description = str;
}

const bool Device::isLoopback() const {
    return loopback;
}

const vector<Packet> Device::getPackets() {
    return packets;
}

//
// overload the ostream << operator for Device
//
 ostream &operator<<(ostream &out, const Device &d) {
     out << d.getName() << ":" << d.getDescription() << ":" << (d.isLoopback()?"LOOPBACK":"");
    return out;
}

//
// Read n packets from this device and load into packet list
// Return the number of packets actually read
//
int Device::capture(int n) {
    
    char error[PCAP_ERRBUF_SIZE] {};
    pcap_t* pcap = pcap_open_live(name.c_str(), snapLen, true, timeout, error);
    if (pcap == NULL) {
        throw runtime_error(error);
    }
    if (pcap_datalink(pcap) != DLT_EN10MB) {
        pcap_close(pcap);
        throw runtime_error("Link-layer type not IEEE 802.3 Ethernet");
    }
    
    const unsigned char * packet = NULL;
    struct pcap_pkthdr header;
    for (int i = 0; i < n; ++i) {
        packet = pcap_next(pcap, &header);
        if (packet != NULL) {
            Packet p(packet);
        }
    }
    
    pcap_close(pcap);
    return (int) packets.size();
}
