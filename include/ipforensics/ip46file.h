/**
 *  @file ip46file.h
 *  @brief IP46File class definitions
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

#ifndef IPFORENSICS_IP46FILE_H_
#define IPFORENSICS_IP46FILE_H_

#include <set>
#include <string>
#include "ipforensics/ip4and6.h"

/**
 *  @brief File-based model class for the IPForensics library, following the
 *         model-view-controller software design pattern
 *  @details IP46File contains all the operations and attributes associated with
 *           the file-based storage and manipulation of IPForensics information
 */
class IP46File {
 private:
  /**
   *  @brief Pointer to the main controller this IP46File is associated with
   */
  IPForensics* ip_;

 public:
  /**
   *  @brief Constructs an IP46File instance with the supplied pointer to 
   *         IPForensics and filename
   *  @param ip the IPForensics instance that this IP46File belongs to
   */
  explicit IP46File(IPForensics* ip);

  /**
   *  @brief Accessor method for the ip_ property
   *  @retval IPForensics* main controller this IP46File is associated with
   */
  IPForensics* ip() const;

  /**
   *  @brief Determines if the file is a valid IPForensics information file
   *  @retval bool true if valid IPForensics output file, false otherwise
   */
  bool valid() const;

  /**
   *  @brief Load hosts from a valid IPForensics information file
   */
  void load();
};

#endif  // IPFORENSICS_IP46FILE_H_
