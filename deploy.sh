#!/bin/bash -e

echo "Reloading Stopping Clamir Linux Daemon"
ssh -t root@$1 'systemctl stop clamir-linux-daemon'

echo "Copying clamir-linux-daemon"
scp build-target/clamir/clamir-linux-daemon root@$1:/usr/bin

echo "Copying clamir-linux-daemon.service"
scp share/clamir-linux-daemon.service root@$1:/lib/systemd/system

echo "Reloading SystemD daemon"
ssh -t root@$1 'systemctl daemon-reload'

echo "Reloading Enabling SysteD Service"
ssh -t root@$1 'systemctl enable clamir-linux-daemon'