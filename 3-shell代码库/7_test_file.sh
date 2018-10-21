#! /bin/bash 

test -d ./1.sh
echo "1\$? = $?"

test -f ./1.sh
echo "2\$? = $?"

touch 8.sh
test -r 8.sh
echo "3\$? = $?"

[ -x 8.sh ]
echo "4\$? = $?"

[ ./1.sh -ot ./8.sh ]
echo "5\$? = $?"

