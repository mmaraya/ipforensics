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

#include <fstream>  // NOLINT
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
  IPForensics ip;
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
    ip.set_verbose(true);
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
  it = find(args.begin(), args.end(), "-c");
  if (it != args.end()) {
    if (next(it) != args.end()) {
      try {
        ip.set_packet_count(stoi(*next(it)));
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
  it = find(args.begin(), args.end(), "-r");
  if (it != args.end()) {
    if (next(it) != args.end()) {
      ip.set_in_file(*next(it));
    } else {
      std::cout << ipf::kProgramName << ": option -r requires an argument\n";
      usage();
      return 1;
    }
  }
  // write host report to -w filename
  it = find(args.begin(), args.end(), "-w");
  if (it != args.end()) {
    if (next(it) != args.end()) {
      ip.set_out_file(*next(it));
      std::ofstream ofs(ip.out_file(), std::ofstream::out);
      if (!ofs.is_open()) {
        std::cout << ipf::kProgramName << ": could not open output file ";
        std::cout << ip.out_file() << std::endl;
        return 1;
      }
      ofs.close();
    } else {
      std::cout << ipf::kProgramName << ": option -w requires an argument\n";
      usage();
      return 1;
    }
  }
  // load hosts from either file or packet capture device
  int packets_loaded {0};
  if (ip.in_file().empty()) {
    ip.set_device(device_name);
    packets_loaded = ip.load_from_device();
  } else {
    packets_loaded = ip.load_from_file();
  }
  // exit if error encountered
  if (packets_loaded < 0) {
    return packets_loaded;
  }
  // display packet count captured
  if (ip.verbose()) {
    std::cout << std::dec << packets_loaded << " packet(s) read.";
    std::cout << std::endl;
  }
  // display hosts and summary or write to a file
  try {
    ip.results();
  } catch (std::exception const &e) {
    std::cout << ipf::kProgramName << ": ";
    std::cout << "Could not save results to specified output file: ";
    std::cout << e.what() << std::endl;
  }
}

/**
 *  @details Display program name, version, and usage
 */
void usage() {
  std::cout << ipf::kProgramName << ", version " << ipf::kMajorVersion << '.';
  std::cout << ipf::kMinorVersion << "\n\n";
  std::cout << "usage: " << ipf::kProgramName;
  std::cout << " [-hv] [-d device] [-n packets] [-f filename]\n";
  std::cout << "-h           display usage\n";
  std::cout << "-v           verbose display\n";
  std::cout << "-i interface packet capture device to use (requires admin)\n";
  std::cout << "-c count     number of packets to read or capture\n";
  std::cout << "-r in file   read packets from pcap file\n";
  std::cout << "-w out file  write host summary to file\n";
  std::cout << std::endl;
}
