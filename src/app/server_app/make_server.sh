#!/bin/sh
SERVER_SRC=../../server
gcc -g -o server_app $SERVER_SRC/../common/device_operations.c $SERVER_SRC/server_controller.c $SERVER_SRC/server_bt.c $SERVER_SRC/server_comm.c server.c -lbluetooth
