#! /bin/bash 

MIN=0

for A in $*
do 
	if [ $MIN -gt $A ]
	then
		MIN=$A
	fi
done

echo "MIN = $MIN"
