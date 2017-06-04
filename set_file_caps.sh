#!/usr/bin/env bash

echo " "
echo "+----------------------------------------+"
echo "| Try to set file capabilities           |"
echo "| --> need sudo for some parts           |"
echo "+----------------------------------------+"
echo " "

sudo -v
# keep-alive
while true; do sudo -n true; sleep 60; kill -0 "$$" || exit; done 2>/dev/null &


# ttenv_gpio
if [ -f ./ttenv_gpio/ttenv_gpio ]; then
    sudo setcap "cap_sys_nice=pe" ./ttenv_gpio/ttenv_gpio
else
    echo "./ttenv_gpio/ttenv_gpio does not exist"
fi

if [ -f ${HOME}/bin/ttenv_gpio ]; then
    sudo setcap "cap_sys_nice=pe" ${HOME}/bin/ttenv_gpio
else
    echo "${HOME}/bin/ttenv_gpio does not exist"
fi

# ttenv_gpio_simple
if [ -f ./ttenv_gpio_simple/ttenv_gpio_simple ]; then
    sudo setcap "cap_sys_nice=pe" ./ttenv_gpio_simple/ttenv_gpio_simple
else
    echo "./ttenv_gpio_simple/ttenv_gpio_simple does not exist"
fi

if [ -f ${HOME}/bin/ttenv_gpio_simple ]; then
    sudo setcap "cap_sys_nice=pe" ${HOME}/bin/ttenv_gpio_simple
else
    echo "${HOME}/bin/ttenv_gpio_simple does not exist"
fi

# ttenv_simple
if [ -f ./ttenv_simple/ttenv_simple ]; then
    sudo setcap "cap_sys_nice=pe" ./ttenv_simple/ttenv_simple
else
    echo "./ttenv_simple/ttenv_simple does not exist"
fi

if [ -f ${HOME}/bin/ttenv_simple ]; then
    sudo setcap "cap_sys_nice=pe" ${HOME}/bin/ttenv_simple
else
    echo "${HOME}/bin/ttenv_simple does not exist"
fi

# ttenv_swt_led
if [ -f ./ttenv_swt_led/ttenv_swt_led ]; then
    sudo setcap "cap_sys_nice=pe" ./ttenv_swt_led/ttenv_swt_led
else
    echo "./ttenv_swt_led/ttenv_swt_led does not exist"
fi

if [ -f ${HOME}/bin/ttenv_swt_led ]; then
    sudo setcap "cap_sys_nice=pe" ${HOME}/bin/ttenv_swt_led
else
    echo "${HOME}/bin/ttenv_swt_led does not exist"
fi

echo " "
echo "+----------------------------------------+"
echo "|          Cheers $USER                |"
echo "+----------------------------------------+"
echo " "
