/**
 *  @file main.cpp
 *  @brief IPForensics program entry point
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
#include "ipforensics/main.h"

/**
 *  @brief IPForensics program entry point
 *  @param argc number of command-line arguments
 *  @param argv array of command-line arguments
 *  @retval int returns 0 upon successful program completion, non-zero otherwise
 */
int main(int argc, char* argv[]) {
  // load arguments into vector<string> so we can use std::find
  std::vector<std::string> args;
  std::vector<std::string>::iterator it;
  for (int i = 1; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  // display -h help
  it = find(args.begin(), args.end(), "-h");
  if ((argc == 1) || (it != args.end())) {
    usage();
    return 0;
  }
  // verbose displays
  it = find(args.begin(), args.end(), "-v");
  if (it != args.end()) {
    verbose = true;
  }
  // use -i interface
  std::string device_name {};
  it = find(args.begin(), args.end(), "-i");
  if (it != args.end()) {
    if (next(it) != args.end()) {
      device_name = *next(it);
    } else {
      std::cout << ipf::kProgramName << ": option -i requires an argument\n";
      usage();
      return 1;
    }
  }
  // capture -c count packets
  int packet_count {};
  it = find(args.begin(), args.end(), "-c");
  if (it != args.end()) {
    if (next(it) != args.end()) {
      try {
        packet_count = stoi(*next(it));
      } catch (std::exception const &e) {
        std::cout << "Could not convert \'-c " << *next(it);
        std::cout << "\' into a number: " << e.what() << std::endl;
        return 1;
      }
    } else {
      std::cout << ipf::kProgramName << ": option -c requires an argument\n";
      usage();
      return 1;
    }
  }
  // read packets from -r filename
  std::string filename {};
  it = find(args.begin(), args.end(), "-r");
  if (it != args.end()) {
    if (next(it) != args.end()) {
      filename = *next(it);
    } else {
      std::cout << ipf::kProgramName << ": option -r requires an argument\n";
      usage();
      return 1;
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
  // exit if error encountered
  if (packets_loaded < 0) {
    return packets_loaded;
  }
  // display packet count captured
  if (verbose) {
    std::cout << std::dec << packets_loaded << " packet(s) read.";
    std::cout << std::endl;
  }
  // display hosts
  std::cout << ipf::kHeader << std::endl;
  for (Host h : ip.hosts()) {
    std::cout << h << std::endl;
  }
}

/**
 *  @details Display program name, version, and usage
 */
void usage() {
  std::cout << ipf::kProgramName << ", version " << ipf::kMajorVersion << '.';
  std::cout << ipf::kMinorVersion << "\n\n";
  std::cout << "usage: " << ipf::kProgramName;
  std::cout << " [-hv] [-d device] [-n packets] [-f filename]" << std::endl;
  std::cout << "-h           display usage" << std::endl;
  std::cout << "-v           verbose display" << std::endl;
  std::cout << "-i interface packet capture device to use" << std::endl;
  std::cout << "-r filename  read packets from pcap file" << std::endl;
  std::cout << "-c count     number of packets to read or capture" << std::endl;
}

/**
 *  @details The list of available packet devices is loaded from the system and
 *           matched against the command-line supplied device name.  Packets are
 *           captured and hosts extracted.
 */
int load_from_device(IPForensics *ip) {
  // load packet capture device list from system
  try {
    ip->load_devices();
  } catch (std::exception const &e) {
    std::cout << ipf::kProgramName << ": ";
    std::cout << "Could not query system for packet capture devices: ";
    std::cout << e.what() << std::endl;
  }
  // select device to use
  Device device(ip);
  for (Device d : ip->devices()) {
    if (ip->device() == d.name()) {
      device = d;
    }
  }
  // exit if invalid device specified
  if (!ip->device().empty() && (ip->device() != device.name())) {
    std::cout << ipf::kProgramName << ": ";
    std::cout << "Invalid packet capture device \'" << ip->device() << "\'. ";
    std::cout << "Valid device(s):\n";
    for (size_t i = 0; i < ip->devices().size(); ++i) {
      std::cout << i+1 << ". " << ip->devices()[i] << '\n';
    }
    std::cout << std::endl;
    return -1;
  }
  // display run-time parameters
  if (verbose) {
    std::cout << "Using \'" << device.name() << "\' with network address ";
    std::cout << device.net() << " and network mask " << device.mask();
    std::cout << " to capture " << ip->packet_count() << " packet(s).";
    std::cout << std::endl;
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

/**
 *  @details Packets are read from the command-line pcap file and hosts are 
 *           extracted from the packets.
 */
int load_from_file(IPForensics *ip) {
  // display run-time parameters
  if (verbose) {
    std::cout << "Reading ";
    if (ip->packet_count() == 0)
      std::cout << "all";
    else
      std::cout << ip->packet_count();
    std::cout << " packet(s) from " << '\'' << ip->filename() << '\'';
    std::cout << std::endl;
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
