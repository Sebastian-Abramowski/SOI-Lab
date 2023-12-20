#!/bin/bash

./prepare.sh
./create_shared_memory
./run_tests.sh
./delete_shared_memory
./clear.sh
