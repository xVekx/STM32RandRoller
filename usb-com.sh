#!/bin/bash
for (( ; ; ))
do
   picocom -b 115200 /dev/ttyACM0
   echo "infinite loops [ hit CTRL+C to stop]"
   sleep 1
done
