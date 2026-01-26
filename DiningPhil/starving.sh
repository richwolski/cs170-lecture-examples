#!/bin/bash

grep eating dp_5_out_32_1_16cores.txt | grep -v longer | awk '{print $3}' | sort -n | uniq

