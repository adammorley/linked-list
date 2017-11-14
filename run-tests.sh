#!/bin/bash

make clean
make all
./node-test && \
    ./list-test && \
    ./thread-test && \
    ./perf-test && \
    echo 'ok'
