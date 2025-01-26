#!/bin/bash

if [[ -z "${TSH_INSTALL_DIR}" ]]; then
	echo "please use 'make remove' to run this script"
	exit
fi

# remove executable
rm "${TSH_INSTALL_DIR}/tsh"

# remove from valid shells
sed -i "/${INSTALL_DIR_PATH}\/tsh/d" /etc/shells
