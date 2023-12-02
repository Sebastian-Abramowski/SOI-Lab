#!/bin/sh
cc see_pid.c -o seepid
cc tst_setgroup.c -o setgroup

chmod +x ./seepid
chmod +x ./setgroup

