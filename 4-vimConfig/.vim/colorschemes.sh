#########################################################################
# File Name: colorschemes.sh
# Author: Hello
# mail: Hello@163.com
# Created Time: 2019年10月14日 星期一 23时31分32秒
#########################################################################
#!/bin/bash


DIR_SRC=/home/$(users)/.vim/bundle/vim-colorschemes/colors
DIR_DEC=/home/$(users)/.vim/colors

sudo cp $DIR_SRC/*  $DIR_DEC
