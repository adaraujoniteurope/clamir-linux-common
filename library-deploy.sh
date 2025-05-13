#!/bin/bash -e

echo "Copying clamir-linux-common"
ssh -t root@$1 'mkdir -p /usr/local/lib'
scp build-target/libclamir-linux-common-shared.so root@$1:/usr/local/lib
scp build-target/libclamir-linux-common-static.a root@$1:/usr/local/lib