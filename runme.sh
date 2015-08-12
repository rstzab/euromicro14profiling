#!/bin/bash
 g++ *.cpp imageProcessing/*.cpp scheduler/*.cpp usageReporter/*.cpp utils/*.cpp -pthread -std=c++11 -pthread -lX11 -lpng -lX11 -o imageProcess
./imageProcess
