#!/bin/sh
gcc -g -o discover ../src/server/server_bt.c ../src/server/server_comm.c bt_discovery_test.c -lbluetooth
