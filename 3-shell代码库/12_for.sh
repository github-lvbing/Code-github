#! /bin/bash 

sum=0

for A in hello world beijing shanghai
do
	echo "A = $A"
	sum=`expr $sum + 1`
done

echo "sum = $sum"
