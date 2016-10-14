#!/bin/bash

Dir=$(pwd)

cd ./client/
make clean_out
cd $Dir

cd ./server/
make clean_out
cd $Dir
