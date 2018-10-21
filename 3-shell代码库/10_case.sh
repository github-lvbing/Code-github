#! /bin/bash 

read A

case $A in
	h|H)
		echo "hello"
		echo "hello"
		;;
	w|W)
		echo "world"
		echo "world"
		;;
	*)
		echo "other"
		;;
esac
