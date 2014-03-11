![ipforensics logo](ipforensics.png "ipforensics") [![Build Status](https://travis-ci.org/mmaraya/ipforensics.png?branch=master)](https://travis-ci.org/mmaraya/ipforensics)
==================================================

ipforensics is a network administration utility that conducts a passive inventory of IPv4 and IPv6 devices on your local network segment.  It can be used to perform passive network enumeration and assist in IPv4 to IPv6 migrations.

Supported Platforms
-------------------

ipforensics runs under Linux, Mac OS X, and Windows. 

Download
--------

Precompiled binaries for the latest version of ipforensics can be downloaded from https://github.com/mmaraya/ipforensics/releases/tag/v0.9.

Usage
-----

    ipforensics [-hv] [-d device] [-f filename] [-n packets]
    -h: display usage
    -v: verbose display
    -i interface: packet capture device to use (requires admin)
    -c count: number of packets to read or capture
    -r in file: read packets from pcap file
    -w out file: write or append summary report to file

To read all packets from a pcap file named mycap.cap, use:

    ipforensics -r mycap.cap

To read the first 100 packets from a pcap file named mycap.cap, use:

    ipforensics -r mycap.cap -c 100
    
To read the first 250 packets from network device eth0, use:

    sudo ipforensics -i eth0 -c 250

To read the first 125 packets from network device eth0 and save to out.txt, use:

    sudo ipforensics -i eth0 -c 125 -w out.txt

License
-------

Please see the file named LICENSE. 

Issues
------

Please submit questions, comments, bugs, enhancement requests at https://github.com/mmaraya/ipforensics/issues.

Disclaimer
----------

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
