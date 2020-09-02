#!/bin/bash

####################
# Lab 1 Exercise 4
# Name:
# Student No:
# Lab Group: 
####################

# fill the below up
hostname=$(uname -n)
kernel_version=$(uname -v)
process_cnt=$(ps axu | wc -l)
user_process_cnt=$(ps -U root | wc -l)
mem_usage=$(free | awk 'FNR == 3 {print $3/($3+$4)*100}')
swap_usage=$(free | awk 'FNR == 3 {print $3/($2*100}')

echo "Hostname: $hostname"
echo "Linux Kernel Version: $kernel_version"
echo "Total Processes: $process_cnt"
echo "User Processes: $user_process_cnt"
echo "Memory Used (%): $mem_usage"
echo "Swap Used (%): $swap_usage"