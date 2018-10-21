#! /bin/bash 

A=13
B=12

test $A -gt $B
echo "1\$? = $?"


test $A -eq 10
echo "2\$? = $?"


test $A -gt 0 -o $A -lt $B
echo "3\$? = $?"
