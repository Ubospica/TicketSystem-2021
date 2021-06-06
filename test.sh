#!/bin/sh
./clear.sh

time sh -c '
  cmake-build-debug/code < data/basic_6/1.in > a.out;
  cmake-build-debug/code < data/basic_6/2.in > a.out;
  cmake-build-debug/code < data/basic_6/3.in > a.out;
  cmake-build-debug/code < data/basic_6/4.in > a.out;
  cmake-build-debug/code < data/basic_6/5.in > a.out;
  cmake-build-debug/code < data/basic_6/6.in > a.out;
  cmake-build-debug/code < data/basic_6/7.in > a.out;
  cmake-build-debug/code < data/basic_6/8.in > a.out;
  cmake-build-debug/code < data/basic_6/9.in > a.out;
  cmake-build-debug/code < data/basic_6/10.in > a.out;
'