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

int main(const int argc, const char * argv[]) {
  
  // check for device name supplied in command-line arguments
  string device_name;
  if (argc > 1) {
    device_name = argv[1];
  }
  
  // check for number of packets to capture in command-line arguments
  int packetCount {25};
  if (argc > 2) {
    try {
      packetCount = stoi(argv[2]);
    } catch (exception const &e) {
      cout << "Could not convert \'" << argv[2] << "\' into a number: " << e.what() << endl;
      return 1;
    }
  }
  
  // load packet capture device list from system
  IPForensics ip;
  try {
    ip.load_devices();
  } catch (exception const &e) {
    cout << "Could not query system for packet capture devices: " << e.what() << endl;
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
  cout << "Using \'" << device.name() << "\' to capture " << packetCount << " packet(s)." << endl;
  
  // capture packets
  int actual_packet_count = device.capture(packetCount);
  
  // display packets captured
  vector<Packet> packets = device.packets();
  for (Packet p : packets) {
    cout << p.mac_src().str() << " -> ";
    cout << p.mac_dst().str() << ' ';
    cout << ipf::hexStr(p.ether_type(), kLengthEtherType) << ' ';
    if (p.ipv4()) {
      cout << p.ipv4_src().str() << " -> " << p.ipv4_dst().str();
    }
    if (p.ipv6()) {
      cout << p.ipv6_src().str() << " -> " << p.ipv6_dst().str();
    }
    cout << endl;
  }
  cout << actual_packet_count << " packet(s) captured." << endl;
  
  // extract hosts from packets
  ip.load_hosts(packets);
  
  // display hosts
  for (Host h : ip.hosts()) {
    cout << h << endl;
  }
  
  return 0;
}
