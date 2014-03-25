#!/bin/sh
SERVER_SRC=../../server
COMMON_SRC=../../common
gcc -g -o c_client_app $COMMON_SRC/device_operations.c $COMMON_SRC/opt_decoder.c $SERVER_SRC/server_bt.c $SERVER_SRC/server_comm.c app_comm.c app_log.c c-app.c main.c -lbluetooth
