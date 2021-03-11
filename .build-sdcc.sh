#!/bin/bash

set -e

# get the source code
svn checkout svn://svn.code.sf.net/p/sdcc/code/trunk
# build and install
cd trunk/sdcc
./configure                    \
         --disable-mcs51-port  \
         --disable-z80-port    \
         --disable-z180-port   \
         --disable-r2k-port    \
         --disable-r3ka-port   \
         --disable-gbz80-port  \
         --disable-tlcs90-port \
         --disable-ds390-port  \
         --disable-ds400-port  \
         --disable-pic14-port  \
         --disable-pic16-port  \
         --disable-hc08-port   \
         --disable-s08-port    \
         --disable-ucsim       \
         --disable-non-free
make -s
sudo make -s install
cd ../..
rm -fRd trunk
