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

#include <fstream>
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
  std::ifstream file(ip_->out_file());
  std::string line;
  if (ip_->verbose()) std::cout << "Reading file " << ip_->out_file() << std::endl;
  if (file.is_open()) {
    while (std::getline(file, line)) {
      std::cout << line << std::endl;
    }
    file.close();
  } else {
    return false;
  }
  return true;
}
