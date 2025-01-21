#!/bin/bash

echo "### market-semaphore ###"
echo "./market-semaphore -c 1 -t 1 -q 1 -s 1 -o 100000"
./market-semaphore -c 1 -t 1 -q 1 -s 1 -o 100000

echo "./market-semaphore -c 1 -t 1 -q 10000 -s 1 -o 100000"
./market-semaphore -c 1 -t 1 -q 10000 -s 1 -o 100000

echo "./market-semaphore -c 1 -t 2 -q 1 -s 1 -o 100000"
./market-semaphore -c 1 -t 2 -q 1 -s 1 -o 100000

echo "./market-semaphore -c 1 -t 2 -q 10000 -s 1 -o 100000"
./market-semaphore -c 1 -t 2 -q 10000 -s 1 -o 100000

echo "/market-semaphore -c 2 -t 2 -q 1 -s 1 -o 100000"
./market-semaphore -c 2 -t 2 -q 1 -s 1 -o 100000

echo "./market-semaphore -c 2 -t 2 -q 10000 -s 1 -o 100000"
./market-semaphore -c 2 -t 2 -q 10000 -s 1 -o 100000

echo "./market-semaphore -c 100 -t 100 -q 100 -s 1 -o 10"
./market-semaphore -c 100 -t 100 -q 100 -s 1 -o 10

