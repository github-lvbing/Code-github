#! /bin/bash 

A=11
B=12

C=`expr $A + $B \* 2`

read A
C=`expr \( $A + $1 \) \* 2`

echo "C = $C"
