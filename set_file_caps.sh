#!/usr/bin/env bash

echo " "
echo "+----------------------------------------+"
echo "|   Try to set file capabilities         |"
echo "+----------------------------------------+"
echo " "

if [ -e example1 ]; then
    sudo setcap "cap_sys_nice=p" ./example1
else
    echo "example1 does not exist"
fi

if [ -e example_gpio ]; then
    sudo setcap "cap_sys_nice=p" ./example_gpio
else
    echo "example_gpio does not exist"
fi

if [ -e example_gpio2 ]; then
    sudo setcap "cap_sys_nice=p" ./example_gpio2
else
    echo "example_gpio2 does not exist"
fi

echo " "
echo "+----------------------------------------+"
echo "|          Cheers $USER                |"
echo "+----------------------------------------+"
echo " "
