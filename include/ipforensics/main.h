/**
 *  @file main.h
 *  @brief IPForensics program header
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

#ifndef IPFORENSICS_MAIN_H
#define IPFORENSICS_MAIN_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include "ipforensics/ip4and6.h"

/** Command-line option to show additional details during program execution */
bool verbose {};

/**
 *  @brief Display command-line program usage and options 
 */
void usage();

/**
 *  @brief Load packets from command-line supplied pcap file 
 *  @param ip Main IPForensics program class
 *  @retval Number of packets read from pcap file or -1 if error detected
 */
int load_from_file(IPForensics *ip);

/**
 *  @brief Load packets from command-line supplied packet capture device
 *  @param ip Main IPForensics program class
 *  @retval Number of packets read from capture device  or -1 if error detected
 */
int load_from_device(IPForensics *ip);

#endif  // IPFORENSICS_MAIN_H