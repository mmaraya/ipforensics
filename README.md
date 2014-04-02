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
    -i interface: packet capture device to use (admin needed)
    -c count: number of packets to read or capture
    -r in file: read packets from pcap file
    -w out file: write summary report to file, or append if the file exists
    -x exclude file: ignore MAC addresses in file

To read all packets from a pcap file named mycap.cap, use:

    ipforensics -r mycap.cap

To read the first 100 packets from a pcap file named mycap.cap, use:

    ipforensics -r mycap.cap -c 100
    
To read the first 250 packets from network device eth0, use:

    sudo ipforensics -i eth0 -c 250

To read the first 125 packets from network device eth0 and write or append results to out.txt, use:

    sudo ipforensics -i eth0 -c 125 -w out.txt
    
Sample Output
-------------

    MAC Address       IPv4 Address    IPv6 Address
    ================= =============== =======================================
    00:23:be:bf:22:ec 192.168.1.102
    00:25:00:ef:54:69 192.168.1.4
    00:26:bb:21:ad:40 192.168.1.9     fe80::226:bbff:fe21:ad40
    00:7f:28:cf:e9:19 108.160.163.43
    6c:c2:6b:22:89:c3 192.168.1.2
    a4:d1:d2:3e:31:f8 192.168.1.6     fe80::841:557d:84c6:a048
    ac:16:2d:bd:36:ec 192.168.1.12    fe80::ae16:2dff:febd:36ec
    dc:2b:61:69:51:e0 192.168.1.3     fe80::104e:a2c9:540:5ba2
    f4:5f:d4:34:a5:ee 192.168.1.100
    =========================================================================
    Hosts: 9; IPv4 only: 5; IPv6 only: 0; dual-stack: 4; migrated: 44%

License
-------

Please see the file named LICENSE. 

Issues
------

Please submit questions, comments, bugs, enhancement requests at https://github.com/mmaraya/ipforensics/issues.

Disclaimer
----------

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
