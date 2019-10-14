#########################################################################
# File Name: fonts.sh
# Author: Hello
# mail: Hello@163.com
# Created Time: 2019年10月14日 星期一 22时56分30秒
#########################################################################
#!/bin/bash

cp /home/lvbing/.vim/fonts/*  /usr/local/share/fonts

cd /usr/local/share/fonts

mkfontscale
mkfontdir
fc-cache -fv
