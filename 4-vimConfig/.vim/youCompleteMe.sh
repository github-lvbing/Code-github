
PROMPT='>>>>>>>>>>>'
DIR_SRC=/home/lvbing/.vim/bundle/YouCompleteMe


echo "$PROMPT install cmake."
sudo apt-get install cmake

echo "$PROMPT install uild-essential."
sudo apt-get install build-essential

echo "$PROMPT install clang."
sudo apt-get install clang

echo "$PROMPT entry src dir."
cd $DIR_SRC


#sudo git submodule update --init --recursive

echo "$PROMPT install ..."
sudo ./install.py  --clang-completer -DUSE_PYTHON2=OFF

echo "$PROMPT search ycm_extra_conf.py."
find -name .ycm_extra_conf.py

