#!/bin/sh
echo "Starting Client App..."
CLIENT_APP=/home/nkigen/NXT/nxtLEGO/src/app/c_app
#start client and pass the motor specs
$CLIENT_APP/c_client_app '-p 6 -m 2:100:2 -n 30'
