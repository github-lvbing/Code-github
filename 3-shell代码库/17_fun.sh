#! /bin/bash 

fun()
{
	local A=19 					#A默认为全局变量，local 使数据变成局部变量
	echo "hello world"
	echo "\$0 = $0"
	echo "\$1 = $1"
	echo "\$2 = $2"
	echo "\$# = $#"

	return $A
}

fun hello world 12 45 78
echo "\$? = $?"
echo "A = $A"

