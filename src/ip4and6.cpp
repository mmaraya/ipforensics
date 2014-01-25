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

std::vector<Device> IPForensics::devices() {
  return devices_;
}

std::set<Host> IPForensics::hosts() {
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

  char error[PCAP_ERRBUF_SIZE];
  pcap_if_t* alldevsp;

  if (pcap_findalldevs(&alldevsp, error) == 0) {
    pcap_if_t* devp = alldevsp;
    while (devp != NULL) {
      Device d = Device();
      unsigned int net {}, mask {};
      d.set_name((devp->name == NULL) ? "" : devp->name);
      d.set_desc((devp->description == NULL) ? "" : devp->description);
      d.set_loopback(devp->flags & PCAP_IF_LOOPBACK);
      if (pcap_lookupnet(d.name().c_str(), &net, &mask, error) == 0) {
        d.set_net(IPv4Address(net));
        d.set_mask(IPv4Address(mask));
      }
      add_device(d);
      devp = devp->next;
    }
  } else {
    pcap_freealldevs(alldevsp);
    throw std::runtime_error(error);
  }
  pcap_freealldevs(alldevsp);
}

//
// Loads all unique hosts from supplied vector<Packet>
//
void IPForensics::load_hosts(Device d) {
  for (Packet p : d.packets()) {
    // add the source host
    std::set<Host>::iterator it = hosts_.find(p.mac_src());
    if (it == hosts_.end()) {
      add_host(p.mac_src(), p.ipv4_src(), p.ipv6_src());
    } else {
      update_host(it, p.ipv4_src(), p.ipv6_src());
    }
    // add the destination host
    it = hosts_.find(p.mac_dst());
    if (it == hosts_.end()) {
      add_host(p.mac_dst(), p.ipv4_dst(), p.ipv6_dst());
    } else {
      update_host(it, p.ipv4_dst(), p.ipv6_dst());
    }
  }
  clean_hosts(d);
}

//
// Add IPv4 or IPv6 host
//
void IPForensics::add_host(MACAddress mac, IPv4Address ipv4, IPv6Address ipv6) {
  hosts_.insert(Host(mac, ipv4, ipv6));
}

//
// Update existing host record with new packet information
//
void IPForensics::update_host(std::set<Host>::iterator it, IPv4Address ipv4,
                              IPv6Address ipv6) {
  Host h = *it;
  if (h.ipv4().address().empty() && !ipv4.address().empty()) {
    h.set_ipv4(ipv4);
  }
  if (h.ipv6().address().empty() && !ipv6.address().empty()) {
    h.set_ipv6(ipv6);
  }
  hosts_.erase(it);
  hosts_.insert(h);
}

//
// Remove broadcast, multicast and non-local hosts
//
void IPForensics::clean_hosts(Device device) {
  std::set<Host>::iterator it;
  for (it = hosts_.begin(); it != hosts_.end(); ) {
    Host host = *it;
    bool remove {false};
    if (host.mac().str() == ipf::kBroadcastMAC) remove = true;
    if (!host.ipv4().address().empty()) {
      if (host.ipv4().str() == ipf::kBroadcastIPv4) remove = true;
      if (!host.ipv4().mask(device.net(), device.mask())) remove = true;
    }
    if (!host.ipv6().address().empty()) {
      if (host.ipv6().address()[0] == 0xFF) remove = true;
    }
    if (remove) {
      hosts_.erase(it++);
    } else {
      ++it;
    }
  }
}
