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

#include "device.h"

Device::Device() {}

const string Device::name() const {
  return name_;
}

void Device::set_name(const string name) {
  name_ = name;
}

const string Device::desc() const {
  return desc_;
}

void Device::set_desc(const string desc) {
  desc_ = desc;
}

const bool Device::loopback() const {
  return loopback_;
}

void Device::set_loopback(const bool loopback) {
  loopback_ = loopback;
}

const IPv4Address Device::net() const {
  return net_;
}

const IPv4Address Device::mask() const {
  return mask_;
}

const vector<Packet> Device::packets() {
  return packets_;
}

//
// overload the ostream << operator for Device
//
ostream &operator<<(ostream &out, const Device &d) {
  out << d.name() << ":" << d.desc() << ":" << (d.loopback() ? "LOOPBACK" : "");
  return out;
}

//
// Read n packets from this device and load into packet list
// Return the number of packets actually read
//
int Device::capture(const int n) {
  
  char error[PCAP_ERRBUF_SIZE] {};
  pcap_t* pcap = pcap_open_live(name_.c_str(), ipf::kSnapLength, true,
                                ipf::kTimeout, error);
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
      packets_.push_back(Packet(packet));
    }
  }
  
  pcap_close(pcap);
  return (int) packets_.size();
}
