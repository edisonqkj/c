#!/bin/bash
[ -e `pwd`/exe ] && rm -r `pwd`/exe
file=$1
if [ -z "${file}" ]; then
	echo "No file for Debuging!"
else
	gcc $file -o exe -std=c99
	if [ -e `pwd`/exe ]; then
		echo "**** Exe Path ****"
		echo "`pwd`/exe"
		echo "******************"
		echo
		echo "***** Print ******"
		./exe
		echo "******************"
	else
		echo "******************"
		echo "     Failure"
		echo "******************"
	fi
fi
