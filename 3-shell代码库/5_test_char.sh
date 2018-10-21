#! /bin/bash 

A=hello
B=hello

test $A != $B
echo "1\$? = $?"

test -z ""
echo "2\$? = $?"
