#!/bin/bash

make clean
make all
./node-test && \
    ./list-test && \
    ./perf-test && \
    echo 'ok'
