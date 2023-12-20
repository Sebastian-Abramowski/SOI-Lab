#!/bin/bash

./client 2.9 1>/dev/null &
pid_client1=$!
./client 2.9 1>/dev/null &
pid_client2=$!
./client 2.9 1>/dev/null &
pid_client3=$!
./client 2.9 1>/dev/null &
pid_client4=$!

./creation 1>/dev/null &
pid_creation1=$!
./creation 1>/dev/null &
pid_creation2=$!
./creation 1>/dev/null &
pid_creation3=$!

./delivery &
pid_courier=$!

./show_queues 50 >./tests.txt 2>&1 &
pid_show_queue=$!

wait $pid_show_queue
kill $pid_courier

kill $pid_creation3
kill $pid_creation2
kill $pid_creation1

kill $pid_client4
kill $pid_client3
kill $pid_client2
kill $pid_client1

echo "Tests done"
