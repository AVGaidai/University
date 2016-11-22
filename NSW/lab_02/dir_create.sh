#!/bin/bash

ipaddr=$1
port=$2

if ! [ -d ./$ipaddr/$port ]; then
    mkdir -p $ipaddr/$port
fi
