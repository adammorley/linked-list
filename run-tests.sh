#!/bin/bash

make clean
make all
./test && \
    ./thread-test && \
    ./perf-test && \
    echo 'ok'
