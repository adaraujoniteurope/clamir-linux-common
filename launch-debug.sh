#!/usr/bin/bash -e

./build.sh
./deploy.sh 192.168.1.177
./debug.sh 192.168.1.177