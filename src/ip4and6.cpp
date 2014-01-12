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

#include "ip4and6.h"

vector<Device> IPForensics::devices() {
  return devices_;
}

set<Host> IPForensics::hosts() {
  return hosts_;
}

//
// Add device to the list
//
void IPForensics::add_device(Device d) {
  devices_.push_back(d);
}

//
// Loads all available packet capture devices into the list
//
void IPForensics::load_devices() {
  pcap_if_t* alldevsp;
  char error[PCAP_ERRBUF_SIZE];
  
  if (pcap_findalldevs(&alldevsp, error) == 0) {
    pcap_if_t* devp = alldevsp;
    while (devp != NULL) {
      string name = (devp->name == NULL) ? "" : devp->name;
      string description = (devp->description == NULL) ? "" : devp->description;
      bool loopback = devp->flags & PCAP_IF_LOOPBACK;
      add_device(Device(name, description, loopback));
      devp = devp->next;
    }
  } else {
    pcap_freealldevs(alldevsp);
    throw runtime_error(error);
  }
  pcap_freealldevs(alldevsp);
  return;
}

//
// Loads all unique hosts from supplied vector<Packet>
//
void IPForensics::load_hosts(const vector<Packet> packets) {
  for (Packet p : packets) {
    string mac = ipf::hexStr(p.mac_src(), ipf::kLengthMAC);
    if (hosts_.find(mac) != hosts_.end()) {
      set<Host>::iterator it = hosts_.find(mac);
      Host h = *it;
      if (h.ipv4().empty() && p.ipv4()) {
        hosts_.erase(it);
        h = Host(mac, ipf::intStr(p.ipv4_src(), ipf::kLengthIPv4), h.ipv6());
        hosts_.insert(h);
      }
      if (h.ipv6().empty() && p.ipv6()) {
        hosts_.erase(it);
        h = Host(mac, h.ipv4(), ipf::hexStr(p.ipv6_src(), ipf::kLengthIPv6));
        hosts_.insert(h);
      }
    } else {
      Host h = Host(ipf::hexStr(p.mac_src(), ipf::kLengthMAC));
      if (p.ipv4()) {
        h.set_ipv4(ipf::intStr(p.ipv4_src(), ipf::kLengthIPv4));
      }
      if (p.ipv6()) {
        h.set_ipv6(ipf::hexStr(p.ipv6_src(), ipf::kLengthIPv6));
      }
      hosts_.insert(h);
    }
  }
  return;
}

