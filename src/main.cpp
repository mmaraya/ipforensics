//
// The MIT License (MIT)
//
// Copyright (c) 2014 Michael Maraya
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "main.h"

int main(const int argc, const char * argv[]) {
    
    // check for device name supplied in command-line arguments
    string deviceName;
    if (argc > 1) {
        deviceName = argv[1];
    }
    
    // check for number of packets to capture in command-line arguments
    int packetCount {10};
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
        ip.loadDevices();
    } catch (exception const &e) {
        cout << "Could not query system for packet capture devices: " << e.what() << endl;
    }

    // select device to use
    Device device;
    for (Device d : ip.getDevices()) {
        if (deviceName == d.getName()) {
            device = d;
        } else {
            if (device.getName().empty() && !d.isLoopback()) {
                device = d;
            }
        }
    }
    if (deviceName != device.getName()) {
        cout << "Invalid packet capture device \'" << deviceName << "\'. ";
        cout << "Valid device(s):";
        for (Device d: ip.getDevices()) {
            cout << ' ' << d.getName();
        }
        cout << endl;
        return 1;
    }
    
    // display accepted run-time parameters
    cout << "Using \'" << device.getName() << "\' to capture " << packetCount << " packet(s)." << endl;
    
    device.capture(packetCount);
    
    return 0;
}
