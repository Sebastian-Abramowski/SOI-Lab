#!/bin/sh
echo "Starting testing, Pid 1 - init process, Pid 0 was excluded, it it not being checked"

echo -e "\nTest who max offspring without ignoring any procceses"
cc who_max_chil.c -o who_max_chil
chmod +x ./who_max_chil
./who_max_chil -999 # Ignoring PID that doesn't exist

echo -e "\nTest who max offspring with ignoring PID 1"
cc who_max_chil.c -o who_max_chil
chmod +x ./who_max_chil
./who_max_chil 1

echo -e "\nTest with creating direct children"
cc max_offspr.c -o max_offspr
chmod +x ./max_offspr
./max_offspr 10 


# cleanup
rm who_max_chil max_offspr
sleep 10
echo "Everything clear"

