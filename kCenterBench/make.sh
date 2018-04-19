#!/bin/bash
rm main
g++ Graph.cpp kCenterBench.cpp algorithms/* -o main -O3 -march=native --std=c++14
