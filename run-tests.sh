#!/bin/bash

make clean
make all
./node-test && \
    ./list-test && \
    echo 'ok'
