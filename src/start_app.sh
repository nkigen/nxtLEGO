#!/bin/sh
CLIENT_APP=/app/c_app
SERVER_APP=/app/server_app

echo "Starting Client App..."
#start client and pass the motor specs
$CLIENT_APP/c_client_app '-p 6 -m 2:100:2 -n 30'

echo "Starting server App"
$SERVER_APP/server_app
