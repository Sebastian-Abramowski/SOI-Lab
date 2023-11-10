#!/bin/sh
echo "Starting testing"
echo "Pid 1 is the init process"


echo -e "\nTest who max children"
cc who_max_chil.c -o who_max_chil
chmod +x ./who_max_chil
./who_max_chil

echo -e "\nTest max children"
cc max_chil.c -o max_chil
chmod +x ./max_chil
./max_chil 5
echo "Waiting for all processes connected to ./max_chil to end"
sleep 11

# test max children but it should return a subprocess of our main process
echo -e "\nTest max children different case"
cc crt_5_chil.c -o crt_5_chil
chmod +x ./crt_5_chil
cc max_chil2.c -o max_chil2
./max_chil2

# cleanup
rm who_max_chil max_chil max_chil2 crt_5_chil

sleep 10
echo "Everything clear..."
