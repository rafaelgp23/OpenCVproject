#!/bin/bash

if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

cd "${0%/*}"
echo A | unzip botan.zip
cd botan
./configure.py
make
LD_LIBRARY_PATH=. ./botan-test
./botan-test
sudo make install
echo "include /usr/local/lib" | sudo tee -a /etc/ld.so.conf
sudo ldconfig
cd ..
sudo rm -rf botan
echo "install-botan script has finished with success"
