#!/bin/bash

# this prevents the script from being executed directly
if [[ -z ${TSH_INSTALL_DIR} ]]; then
	echo "please use 'make install' to run this script"
	exit
fi
if [[ $(id -u) != 0 ]]; then
	echo "please execute as root"
	exit
fi

# copy executable
cp ./tsh "${TSH_INSTALL_DIR}/tsh"

# add to /etc/shells
if grep -q "${TSH_INSTALL_DIR}/tsh" /etc/shells; then
	echo "tsh is already a valid shell"
else
	echo "/bin/tsh" >> /etc/shells
fi

unset TSH_INSTALL
