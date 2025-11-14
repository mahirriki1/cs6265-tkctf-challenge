#!/bin/bash

# Disable ASLR
echo 0 > /proc/sys/kernel/randomize_va_space

# Drop privileges to ctf user and run the binary
exec su ctf -c "socat -T60 TCP-LISTEN:9999,reuseaddr,fork EXEC:'timeout 60 /target'"
