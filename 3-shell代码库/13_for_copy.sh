#! /bin/bash 

if [ -d dir ]
then
	echo "exist"
else
	mkdir dir
fi

sum=0

for file in `ls`
do 
	if [ -f $file ]
	then
		cp $file dir
		sum=`expr $sum + 1`
	fi
done

echo "sum = $sum"
