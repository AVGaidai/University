#!/bin/bash

Dir=$(pwd)

cd ./client/
make
cd $Dir

cd ./server/
make
cd $Dir
