#!/bin/bash

if [ $# -eq 0 ]
then
	BITRT=250000
else
	BITRT=$1
fi

echo bitrate=$BITRT
ifconfig can0 down
ip link set can0 type can bitrate $BITRT
ifconfig can0 up
