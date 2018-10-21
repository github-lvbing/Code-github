#! /bin/bash 

fun()
{
	A=32 					#A默认为局部变量
	echo "hello world"
	echo "hello world"
	echo "hello world"
	echo "\$0 = $0"
	echo "\$1 = $1"
	echo "\$2 = $2"
	echo "\$# = $#"

	return $A
}

x=`fun beijing shanghai 100 3`
echo "\$? = $?"
echo "$x"
echo "A = $A"
