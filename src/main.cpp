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
  cout << ipf::kProgramName << ", version " << ipf::kMajorVersion << '.';
  cout << ipf::kMinorVersion << "\n\n";
  cout << "usage: " << ipf::kProgramName << " [-h] [-d device] [-n packets]";
  cout << endl;
}

//
// ipforensics
//
int main(int argc, char * argv[]) {
  
  // load arguments into vector<string> so we can use std::find
  vector<string> args;
  vector<string>::iterator it;
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
  string device_name {};
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
      } catch (exception const &e) {
        cout << "Could not convert \'-n " << *next(it) << "\' into a number: ";
        cout << e.what() << endl;
        return 1;
      }
    }
  }
  
  // load packet capture device list from system
  IPForensics ip;
  try {
    ip.load_devices();
  } catch (exception const &e) {
    cout << "Could not query system for packet capture devices: ";
    cout << e.what() << endl;
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
    cout << "Invalid packet capture device \'" << device_name << "\'. ";
    cout << "Valid device(s):";
    for (Device d: ip.devices()) {
      cout << ' ' << d.name();
    }
    cout << endl;
    return 1;
  }
  
  // display accepted run-time parameters
  cout << "Using \'" << device.name() << "\' with network address ";
  cout << device.net() << " and network mask " << device.mask();
  cout << " to capture " << packetCount << " packet(s)." << endl;
  
  // capture packets
  int actual_packet_count = device.capture(packetCount);
  
  // display packets captured
  for (Packet p : device.packets()) {
    cout << p.mac_src() << " -> " << p.mac_dst() << ' ';
    cout << ipf::hexStr(p.ether_type(), kLengthEtherType) << ' ';
    if (p.ipv4()) {
      cout << p.ipv4_src() << " -> " << p.ipv4_dst();
    }
    if (p.ipv6()) {
      cout << p.ipv6_src() << " -> " << p.ipv6_dst();
    }
    cout << endl;
  }
  cout << actual_packet_count << " packet(s) captured." << endl;
  
  // extract hosts
  ip.load_hosts(device);
  
  // display hosts
  for (Host h : ip.hosts()) {
    cout << h << endl;
  }
}
