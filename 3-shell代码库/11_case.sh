#! /bin/bash 

read A

if [ $A -gt 100 -o $A -lt 0 ]
then
	echo "other"
	exit
fi

A=`expr $A / 10`

case $A in
	10|9)
		echo "A"
		;;
	8)
		echo "B"
		;;
	7)
		echo "C"
		;;
	6)
		echo "D"
		;;
	*)
		echo "E"
		;;
esac

