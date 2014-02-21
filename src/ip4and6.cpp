/**
 *  @file ip4and6.cpp
 *  @brief IPForensics class implementation
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
#include <set>
#include "ipforensics/ip4and6.h"

std::vector<Device> IPForensics::devices() const {
  return devices_;
}

std::set<Host> IPForensics::hosts() const {
  return hosts_;
}

std::string IPForensics::device() const {
  return device_;
}

std::string IPForensics::filename() const {
  return filename_;
}

int IPForensics::packet_count() const {
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

/**
 *  @details Loads all available network devices from the host system, setting
 *           each device's name, description, loopback status, network address
 *           and network mask
 *  @throws std::runtime_error if an error is encountered when attempting to 
 *          list all network devices with the details from libpcap
 */
void IPForensics::load_devices() {
  char error[PCAP_ERRBUF_SIZE];
  pcap_if_t* alldevsp;
  if (pcap_findalldevs(&alldevsp, error) == 0) {
    pcap_if_t* devp = alldevsp;
    while (devp != NULL) {
      Device d = Device(this);
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

/**
 *  @details Iterates through all the packets in the supplied Device and loads
 *           the source host and destination host from each packet.  If a host
 *           detected from the packets already exists in the collection, the
 *           host is updated with any new information from the packet (the IPv4
 *           or IPv6 address if not previously found).
 */
void IPForensics::load_hosts(Device device) {
  for (Packet packet : device.packets()) {
    // add the source host
    auto it = hosts_.find(static_cast<Host>(packet.mac_src()));
    if (it == hosts_.end()) {
      add_host(packet.mac_src(), packet.ipv4_src(), packet.ipv6_src());
    } else {
      update_host(it, packet.ipv4_src(), packet.ipv6_src());
    }
    // add the destination host
    it = hosts_.find(static_cast<Host>(packet.mac_dst()));
    if (it == hosts_.end()) {
      add_host(packet.mac_dst(), packet.ipv4_dst(), packet.ipv6_dst());
    } else {
      update_host(it, packet.ipv4_dst(), packet.ipv6_dst());
    }
  }
  // remove multicast and broadcast hosts
  IPv4Address net = device.net(), mask = device.mask();
  clean_hosts(&net, &mask);
}

/**
 *  @todo Add command-line parameters for IPv4 network address and mask so we 
 *        can remove broadcast and multicast hosts from the result
 */
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
  // if packet_count_ is set, read specified number of packets only
  if (packet_count_ > 0) {
    for (int i = 0; i < packet_count_; ++i) {
      packet = pcap_next(pcap, &header);
      if (packet != NULL) {
        packets_.push_back(Packet(packet));
      }
    }
  } else {
    // if packet_count_ is not set, read all packets
    packet = pcap_next(pcap, &header);
    while (packet != NULL) {
      packets_.push_back(Packet(packet));
      packet = pcap_next(pcap, &header);
    }
  }
  // close the packet capture
  pcap_close(pcap);
  // extract hosts from packets
  for (Packet p : packets_) {
    // add the source host
    std::set<Host>::iterator it = hosts_.find(static_cast<Host>(p.mac_src()));
    if (it == hosts_.end()) {
      add_host(p.mac_src(), p.ipv4_src(), p.ipv6_src());
    } else {
      update_host(it, p.ipv4_src(), p.ipv6_src());
    }
    // add the destination host
    it = hosts_.find(static_cast<Host>(p.mac_dst()));
    if (it == hosts_.end()) {
      add_host(p.mac_dst(), p.ipv4_dst(), p.ipv6_dst());
    } else {
      update_host(it, p.ipv4_dst(), p.ipv6_dst());
    }
  }
  // remove meaningless hosts
  clean_hosts(nullptr, nullptr);
}

void IPForensics::add_host(MACAddress mac, IPv4Address ipv4, IPv6Address ipv6) {
  hosts_.insert(Host(mac, ipv4, ipv6));
}

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

/**
 *  @details This helper method removes "fake" hosts from IPForensics::hosts_
 *           such as multicast and broadcast addresses.
 */
void IPForensics::clean_hosts(IPv4Address* net, IPv4Address *mask) {
  std::set<Host>::iterator it;
  for (it = hosts_.begin(); it != hosts_.end(); ) {
    Host host = *it;
    bool remove {false};
    if (host.mac().fake() || host.ipv4().fake() || host.ipv6().fake()) {
      remove = true;
    }
    if (!host.ipv4().address().empty() && net != nullptr) {
      if (!host.ipv4().mask(*net, *mask)) {
        remove = true;
      }
    }
    if (remove) {
      hosts_.erase(it++);
    } else {
      ++it;
    }
  }
}
