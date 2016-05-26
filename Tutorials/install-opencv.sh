#!/bin/bash

if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi


###########################################################
#
# OpenCV - install
# http://opencv.org/
#
###########################################################

#----------------------------------------------------------
# Create a logger to register the installation
#----------------------------------------------------------
# Start time
dateformat="+%a %b %-eth %Y %I:%M:%S %p %Z"
starttime=$(date "$dateformat")
starttimesec=$(date +%s)

# Get current directory
curdir=$(cd `dirname $0` && pwd)

# Create a file where the actions will be logged
logfile="$curdir/install-opencv.log"
rm -f $logfile

# Simple logger
log(){
	timestamp=$(date +"%Y-%m-%d %k:%M:%S")
	echo "\n$timestamp $1"
	echo "$timestamp $1" >> $logfile 2>&1
}
 
# Starting installation
log "Starting OpenCV installation"


#----------------------------------------------------------
# Ensuring an updated environment
#----------------------------------------------------------

# Inform the next action
log "Executing apt-get update and apt-get upgrade"
 
sudo apt-get update
sudo apt-get upgrade

 
#----------------------------------------------------------
# Dependencies
#----------------------------------------------------------

log "Installing dependencies"
 
sudo apt-get -y install libopencv-dev build-essential cmake git libgtk2.0-dev pkg-config python-dev python-numpy libdc1394-22 libdc1394-22-dev libjpeg-dev libpng12-dev libtiff4-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libxine-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev libtbb-dev libqt4-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils unzip
 
 
#----------------------------------------------------------
# Installing OpenCV
#----------------------------------------------------------

log "Downloading OpenCV"

# Get source code
git clone https://github.com/Itseez/opencv.git

log "Installing OpenCV"
 
# Enter directory
cd opencv
 
# Create directory: 'build'
mkdir build
 
# Enter directory
cd build
 
# Build the project using CMake
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_OPENGL=ON ..
 
# Compile the project
make
 
# Install the libraries in appropriate folder
sudo make install
 
# Add the OpenCV library path to the Ubuntu default libraries path
sudo /bin/bash -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
 
# Update the default Ubuntu paths
sudo ldconfig
 
# OpenCV successfully installed!
log "OpenCV successfully installed!"

#----------------------------------------------------------
# Display elapsed time from this process
#----------------------------------------------------------

# End time
endtime=$(date "$dateformat")
endtimesec=$(date +%s)

elapsedtimesec=$(expr $endtimesec - $starttimesec)
ds=$((elapsedtimesec % 60))
dm=$(((elapsedtimesec / 60) % 60))
dh=$((elapsedtimesec / 3600))
displaytime=$(printf "%02d:%02d:%02d" $dh $dm $ds)
log "Time spent: $displaytime\n"
