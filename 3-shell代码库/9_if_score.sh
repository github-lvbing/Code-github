#! /bin/bash 

read A

if [ $A -gt 100 -o $A -lt 0 ]
then
	echo "error"
	exit 					#退出程序
fi

if [ $A -le 100 -a $A -ge 90 ]
then
	echo "A"
fi

if [ $A -le 89 -a $A -ge 80 ]
then
	echo "B"
fi

if [ $A -le 79 -a $A -ge 70 ]
then
	echo "C"
fi

if [ $A -le 69 -a $A -ge 60 ]
then
	echo "D"
fi

if [ $A -le 59 -a $A -ge 0 ]
then
	echo "E"
fi


