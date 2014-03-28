/**
 *  @file ip46file.cpp
 *  @brief IP46File class implementation
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

#include <fstream>  // NOLINT
#include <set>
#include <string>
#include "ipforensics/ip46file.h"

IP46File::IP46File(IPForensics* ip) {
  ip_ = ip;
}

IPForensics* IP46File::ip() const {
  return ip_;
}

bool IP46File::valid() const {
  if (ip_->out_file().empty() || ip_ == nullptr) return false;
  std::ifstream fs(ip_->out_file());
  if (fs.is_open() == false) return false;
  std::string line;
  std::getline(fs, line);
  if (line != ipf::kHeader1) return false;
  std::getline(fs, line);
  if (line != ipf::kHeader2) return false;
  bool end = false;
  while (std::getline(fs, line)) {
    if (line == ipf::kFooter1) {
      end = true;
      break;
    }
    if (!std::regex_search(line, ipf::kMACRegEx)) return false;
  }
  fs.close();
  return end;
}

void IP46File::load() {
  std::ifstream fs(ip_->out_file());
  if (fs.is_open()) {
    std::string line, mac_str, v4_str, v6_str;
    std::getline(fs, line);
    std::getline(fs, line);
    while (std::getline(fs, line)) {
      if (line == ipf::kFooter1) {
        break;
      }
      Host host;
      MACAddress mac;
      mac_str = line.substr(ipf::kOutputOffsetMAC, ipf::kOutputLengthMAC);
      if (mac_str.find_first_not_of(' ') != std::string::npos) {
        mac = MACAddress(mac_str);
        host = Host(mac);
      }
      IPv4Address v4;
      v4_str = line.substr(ipf::kOutputOffsetIPv4, ipf::kOutputLengthIPv4);
      if (v4_str.find_first_not_of(' ') != std::string::npos) {
        v4 = IPv4Address(v4_str);
        host.set_ipv4(v4);
      }
      IPv6Address v6;
      v6_str = line.substr(ipf::kOutputOffsetIPv6, ipf::kOutputLengthIPv6);
      if (v6_str.find_first_not_of(' ') != std::string::npos) {
        v6 = IPv6Address(v6_str);
        host.set_ipv6(v6);
      }
      ip_->add_host(host);
      if (ip_->verbose()) {
        std::cout << "Loaded host " << host << std::endl;
      }
    }
  }
}
