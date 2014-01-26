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

std::string IPForensics::device() {
  return device_;
}

std::string IPForensics::filename() {
  return filename_;
}

int IPForensics::packet_count() {
  return packet_count_;
}

std::vector<Packet> IPForensics::packets() {
  return packets_;
}

void IPForensics::set_device(std::string device) {
  device_ = device;
}

void IPForensics::set_filename(std::string filename) {
  filename_ = filename;
}

void IPForensics::set_packet_count(int packet_count) {
  packet_count_ = packet_count;
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
      devices_.push_back(d);
      devp = devp->next;
    }
  } else {
    pcap_freealldevs(alldevsp);
    throw std::runtime_error(error);
  }
  pcap_freealldevs(alldevsp);
}

//
// Loads all unique hosts from packets captured by device
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
  IPv4Address net = d.net(), mask = d.mask();
  clean_hosts(&net, &mask);
}

//
// Loads all unique hosts from user-supplied pcap file
//
void IPForensics::load_hosts(std::string filename) {
 
  // open the filename
  char error[PCAP_ERRBUF_SIZE] {};
  pcap_t* pcap = pcap_open_offline(filename.c_str(), error);
  if (pcap == NULL) {
    throw std::runtime_error(error);
  }
  
  // exit if the data link is not Ethernet
  if (pcap_datalink(pcap) != DLT_EN10MB) {
    pcap_close(pcap);
    throw std::runtime_error("Link-layer type not IEEE 802.3 Ethernet");
  }
  
  // read the packets from the file
  const unsigned char * packet = NULL;
  struct pcap_pkthdr header;
  for (int i = 0; i < packet_count_; ++i) {
    packet = pcap_next(pcap, &header);
    if (packet != NULL) {
      packets_.push_back(Packet(packet));
    }
  }
  
  // close the packet capture
  pcap_close(pcap);
  
  // extract hosts from packets
  for (Packet p : packets_) {
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

  // remove meaningless hosts
  clean_hosts(nullptr, nullptr);
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
void IPForensics::clean_hosts(IPv4Address* net, IPv4Address *mask) {
  std::set<Host>::iterator it;
  for (it = hosts_.begin(); it != hosts_.end(); ) {
    Host host = *it;
    bool remove {false};
    // remove MAC broadcast addresses
    if (host.mac().str() == ipf::kBroadcastMAC) remove = true;
    if (!host.ipv4().address().empty()) {
      // remove IPv4 broadcast addresses
      if (host.ipv4().str() == ipf::kBroadcastIPv4) remove = true;
      // remove IPv4 multicast addresses
      unsigned char prefix = host.ipv4().address()[0] >> 4;
      if ((prefix & ipf::kMulticastIPv4) == ipf::kMulticastIPv4) remove = true;
      // remove addresses that are not within our subnet (if using device)
      if (net != nullptr) {
        if (!host.ipv4().mask(*net, *mask)) remove = true;
      }
    }
    // remove IPv6 multicast addresses
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
