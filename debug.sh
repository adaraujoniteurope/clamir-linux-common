#!/usr/bin/bash -e

ssh -t root@$1 'CONFIGURATION_DIRECTORY=/home/clamir/conf gdbserver 0.0.0.0:2000 /usr/local/bin/clamir-linux-server-dbus'

# echo "Starting SysteD Start"
# ssh -t root@$1 'systemctl start clamir-linux-daemon'

# ssh -t root@$1 'gdbserver 0.0.0.0:2000 /usr/bin/clamir-linux-daemon `pidof clamir-linux-daemon`'
