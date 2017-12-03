#!/bin/bash

make clean
make all
./list-test && \
    ./thread-test && \
    ./perf-test && \
    echo 'ok'
