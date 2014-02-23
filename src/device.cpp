/**
 *  @file device.cpp
 *  @brief Device class implementation
 */

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Michael Maraya
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <string>
#include <vector>
#include "ipforensics/ip4and6.h"
#include "ipforensics/device.h"

Device::Device(IPForensics* ipf) {
  ipf_ = ipf;
}

std::string Device::name() const {
  return name_;
}

std::string Device::desc() const {
  return desc_;
}

bool Device::loopback() const {
  return loopback_;
}

IPv4Address Device::net() const {
  return net_;
}

IPv4Address Device::mask() const {
  return mask_;
}

std::vector<Packet> Device::packets() const {
  return ipf_->packets();
}

void Device::set_name(const std::string name) {
  name_ = name;
}

void Device::set_desc(const std::string desc) {
  desc_ = desc;
}

void Device::set_loopback(const bool loopback) {
  loopback_ = loopback;
}

void Device::set_net(const IPv4Address net) {
  net_ = net;
}

void Device::set_mask(const IPv4Address mask) {
  mask_ = mask;
}

/**
 * @details This method currently only handles Ethernet frames so an exception 
 *          will be thrown if other types are detected
 * @throw std::runtime_error if the packet capture could not be opened or if the 
 *        link-layer header type for the live capture is not IEEE 802.3 Ethernet
 */
int Device::capture(const int n) {
  char error[PCAP_ERRBUF_SIZE] {};
  pcap_t* pcap = pcap_open_live(name_.c_str(), ipf::kSnapLength, true,
                                ipf::kTimeout, error);
  if (pcap == NULL) {
    throw std::runtime_error(error);
  }
  if (pcap_datalink(pcap) != DLT_EN10MB) {
    pcap_close(pcap);
    throw std::runtime_error("Link-layer type not IEEE 802.3 Ethernet");
  }
  const unsigned char * packet = NULL;
  struct pcap_pkthdr header;
  for (int i = 0; i < n; ++i) {
    packet = pcap_next(pcap, &header);
    if (packet != NULL) {
      ipf_->packets_.push_back(Packet(packet));
    }
  }
  pcap_close(pcap);
  return static_cast<int>(packets().size());
}

std::ostream &operator<<(std::ostream &out, const Device &d) {
  out << d.name();
  out << " (" << (d.desc().empty() ? "No description" : d.desc())  << ") ";
  out << (d.loopback() ? "LOOPBACK" : "");
  return out;
}
