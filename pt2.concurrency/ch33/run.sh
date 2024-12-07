#!/bin/sh

killall server

if [[ ! -d "build" ]];then
	mkdir build
fi

gcc -o build/server server.c
gcc -o build/client client.c

./build/server &
sleep 1

./build/client
