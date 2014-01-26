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
// ipforensics
//
int main(int argc, char * argv[]) {
  
  // load arguments into vector<string> so we can use std::find
  std::vector<std::string> args;
  std::vector<std::string>::iterator it;
  for (int i = 1; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  
  // display -h help
  it = find(args.begin(), args.end(), "-h");
  if (it != args.end()) {
    usage();
    return 0;
  }

  // verbose displays
  it = find(args.begin(), args.end(), "-v");
  if (it != args.end()) {
    verbose = true;
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
  int packet_count {10};
  it = find(args.begin(), args.end(), "-n");
  if (it != args.end()) {
    if (next(it) != args.end()) {
      try {
        packet_count = stoi(*next(it));
      } catch (std::exception const &e) {
        std::cout << "Could not convert \'-n " << *next(it) << "\' into a number: ";
        std::cout << e.what() << std::endl;
        return 1;
      }
    }
  }
  
  // read packets from -f filename
  std::string filename {};
  it = find(args.begin(), args.end(), "-f");
  if (it != args.end()) {
    if (next(it) != args.end()) {
      filename = *next(it);
    }
  }
  
  // load hosts from either file or packet capture device
  IPForensics ip;
  ip.set_packet_count(packet_count);
  int packets_loaded {0};
  if (filename.empty()) {
    ip.set_device(device_name);
    packets_loaded = load_from_device(&ip);
  } else {
    ip.set_filename(filename);
    packets_loaded = load_from_file(&ip);
  }
  
  // display packet count captured
  if (verbose) {
    std::cout << std::dec << packets_loaded << " packet(s) read.";
    std::cout << std::endl;
  }
  
  // display hosts
  std::cout << ipf::kNormalHeader << std::endl;
  for (Host h : ip.hosts()) {
    std::cout << h << std::endl;
  }
}

//
// display program usage
//
void usage() {
  std::cout << ipf::kProgramName << ", version " << ipf::kMajorVersion << '.';
  std::cout << ipf::kMinorVersion << "\n\n";
  std::cout << "usage: " << ipf::kProgramName;
  std::cout << " [-hv] [-d device] [-n packets] [-f filename]";
  std::cout << std::endl;
}

//
// load packets from capture device
//
int load_from_device(IPForensics *ip) {
  
  // load packet capture device list from system
  try {
    ip->load_devices();
  } catch (std::exception const &e) {
    std::cout << "Could not query system for packet capture devices: ";
    std::cout << e.what() << std::endl;
  }
  
  // select device to use
  Device device;
  for (Device d : ip->devices()) {
    if (ip->device() == d.name()) {
      device = d;
    } else {
      if (device.name().empty() && !d.loopback()) {
        device = d;
      }
    }
  }
  
  // exit if no device specified or invalid
  if (ip->device() != device.name()) {
    std::cout << "Invalid packet capture device \'" << ip->device() << "\'. ";
    std::cout << "Valid device(s):";
    for (Device d: ip->devices()) {
      std::cout << ' ' << d.name();
    }
    std::cout << std::endl;
    return 1;
  }
  
  // display run-time parameters
  if (verbose) {
    std::cout << "Using \'" << device.name() << "\' with network address ";
    std::cout << device.net() << " and network mask " << device.mask();
    std::cout << " to capture " << ip->packet_count() << " packet(s)." << std::endl;
  }
  
  // capture packets
  int packet_count = device.capture(ip->packet_count());
  
  // display packets captured
  if (verbose) {
    for (Packet p : device.packets()) {
      std::cout << p << std::endl;
    }
  }
  
  // extract hosts
  ip->load_hosts(device);
  return packet_count;
}

//
// load packets from pcap file
//
int load_from_file(IPForensics *ip) {

  // display run-time parameters
  if (verbose) {
    std::cout << "Reading " << ip->packet_count() << " packet(s) from ";
    std::cout << '\'' << ip->filename() << '\'' << std::endl;
  }
  
  // extract packets and hosts from file
  ip->load_hosts(ip->filename());
  
  // display packets read
  if (verbose) {
    for (Packet p : ip->packets()) {
      std::cout << p << std::endl;
    }
  }
  
  // return number of packets read
  return int (ip->packets().size());
}