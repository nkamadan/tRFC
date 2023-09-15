#!/bin/bash

# Install cpufrequtils if not already installed
if ! command -v cpufreq-set &> /dev/null; then
    echo "Installing cpufrequtils..."
    sudo apt-get update
    sudo apt-get install cpufrequtils
fi

# Set each CPU core's frequency and governor
TARGET_FREQUENCY=1500000  # 1.5 GHz

for i in /sys/devices/system/cpu/cpu[0-39]*; do
    cpu_num=$(basename $i | tr -cd '0-9')
    echo "cpu numarasi: ${cpu_num}"
    # Set the frequency
    #sudo cpufreq-set -c $cpu_num -f ${TARGET_FREQUENCY}kHz
    
    # Set the governor to userspace to maintain the frequency
    sudo cpufreq-set -c $cpu_num -g performance
done

#echo "All CPU cores are now set to ${TARGET_FREQUENCY}kHz."
echo "All CPU cores are now set to performance governor"
