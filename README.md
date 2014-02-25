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
    -i interface: packet capture device to use
    -r file: read packets from pcap file
    -c count: number of packets to read or capture

License
-------

Please see the file named LICENSE. 

Issues
------

Please submit questions, comments, bugs, enhancement requests at https://github.com/mmaraya/ipforensics/issues.

Disclaimer
----------

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
