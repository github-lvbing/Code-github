#! /bin/bash 

A=10

while [ $A -ne 0 ]
do
	> file$A.txt
	A=`expr $A - 1`

	if [ $A -eq 5 ] 
	then
#		break 				//结束整个循环
# 		continue 			//跳出本轮循环，继续执行下一次循环
	fi

	echo "A = $A"
done
