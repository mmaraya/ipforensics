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

#include <iostream>
#include <vector>
#include <pcap.h>
#include <pcap/bpf.h>
#include <stdexcept>
#include "packet.h"

class Device {
private:
  std::string name_;
  std::string desc_;
  bool loopback_;
  IPv4Address net_;
  IPv4Address mask_;
  std::vector<Packet> packets_;
public:
  Device();
  const std::string name() const;
  const std::string desc() const;
  const bool loopback() const;
  const IPv4Address net() const;
  const IPv4Address mask() const;
  const std::vector<Packet> packets();
  void set_name(const std::string);
  void set_desc(const std::string);
  void set_loopback(const bool);
  void set_net(const IPv4Address);
  void set_mask(const IPv4Address);
  int capture(const int);
};

std::ostream &operator<<(std::ostream &out, const Device &d);
