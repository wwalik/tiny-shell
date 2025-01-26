#!/bin/bash

echo ${TSH_INSTALL_DIR}

# this prevents the script from being executed directly
if [[ -z ${TSH_INSTALL_DIR} ]]; then
	echo "please use 'make install' to run this script"
	exit
fi

# copy executable
cp ./tsh "${TSH_INSTALL_DIR}/tsh"

# add to /etc/shells
if grep -q "${TSH_INSTALL_DIR}/tsh" /etc/shells; then
	echo "tsh is already a valid shell"
else
	echo "${TSH_INSTALL_DIR}/tsh" >> /etc/shells
fi

unset TSH_INSTALL
