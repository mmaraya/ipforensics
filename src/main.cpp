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

#include "main.h"

//
// display program usage
//
void usage() {
  std::cout << ipf::kProgramName << ", version " << ipf::kMajorVersion << '.';
  std::cout << ipf::kMinorVersion << "\n\n";
  std::cout << "usage: " << ipf::kProgramName << " [-h] [-d device] [-n packets]";
  std::cout << std::endl;
}

//
// ipforensics
//
int main(int argc, char * argv[]) {
  
  // load arguments into vector<string> so we can use std::find
  std::vector<std::string> args;
  std::vector<std::string>::iterator it;
  for (int i = 1; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  
  // display help
  it = find(args.begin(), args.end(), "-h");
  if (it != args.end()) {
    usage();
    return 0;
  }
  
  // use -d device
  std::string device_name {};
  it = find(args.begin(), args.end(), "-d");
  if (it != args.end()) {
    if (next(it) != args.end()) {
      device_name = *next(it);
    }
  }
  
  // capture -n packets
  int packetCount {10};
  it = find(args.begin(), args.end(), "-n");
  if (it != args.end()) {
    if (next(it) != args.end()) {
      try {
        packetCount = stoi(*next(it));
      } catch (std::exception const &e) {
        std::cout << "Could not convert \'-n " << *next(it) << "\' into a number: ";
        std::cout << e.what() << std::endl;
        return 1;
      }
    }
  }
  
  // load packet capture device list from system
  IPForensics ip;
  try {
    ip.load_devices();
  } catch (std::exception const &e) {
    std::cout << "Could not query system for packet capture devices: ";
    std::cout << e.what() << std::endl;
  }
  
  // select device to use
  Device device;
  for (Device d : ip.devices()) {
    if (device_name == d.name()) {
      device = d;
    } else {
      if (device.name().empty() && !d.loopback()) {
        device = d;
      }
    }
  }
  
  // exit if no device specified or invalid
  if (device_name != device.name()) {
    std::cout << "Invalid packet capture device \'" << device_name << "\'. ";
    std::cout << "Valid device(s):";
    for (Device d: ip.devices()) {
      std::cout << ' ' << d.name();
    }
    std::cout << std::endl;
    return 1;
  }
  
  // display accepted run-time parameters
  std::cout << "Using \'" << device.name() << "\' with network address ";
  std::cout << device.net() << " and network mask " << device.mask();
  std::cout << " to capture " << packetCount << " packet(s)." << std::endl;
  
  // capture packets
  int actual_packet_count = device.capture(packetCount);
  
  // display packets captured
  for (Packet p : device.packets()) {
    std::cout << p.mac_src() << " -> " << p.mac_dst() << ' ';
    std::cout << ipf::hexStr(p.ether_type(), ipf::kLengthEtherType) << ' ';
    if (p.ipv4()) {
      std::cout << p.ipv4_src() << " -> " << p.ipv4_dst();
    }
    if (p.ipv6()) {
      std::cout << p.ipv6_src() << " -> " << p.ipv6_dst();
    }
    std::cout << std::endl;
  }
  std::cout << actual_packet_count << " packet(s) captured." << std::endl;
  
  // extract hosts
  ip.load_hosts(device);
  
  // display hosts
  std::cout << ipf::kNormalHeader << std::endl;
  for (Host h : ip.hosts()) {
    std::cout << h << std::endl;
  }
}
