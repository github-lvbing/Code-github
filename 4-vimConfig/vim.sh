

DISCLEAN_OPEN=FALSE
PROMPT='>>>>>>>>>>>'
USER=$(users)
DIR_SRC=/usr/local/src/vim
DIR_INSTALL=/usr/local/install/vim81
DIR_PYTHON3=/usr/lib/python3.6/config-3.6m-x86_64-linux-gnu
DIR_PYTHON2=/usr/lib/python2.7/config-x86_64-linux-gnu
FILE_ENV=/etc/environment
FILE_USER_BASHRC=/home/$USER/.bashrc
FILE_ROOT_BASHRC=/root/.bashrc

STRING_PATH_CONFIG="PATH=\$PATH:$DIR_INSTALL/bin"

#---------------------------------
echo "$PROMPT remove vim:"
sudo apt-get remove --purge vim \
 vi \
 vim-runtime \
 vim-tiny \
 vim-common \
 vim-gui-common \
 vim-doc \
 vim-scripts

echo "$PROMPT remove install dir:"
rm -r $DIR_INSTALL

echo "$PROMPT install Dependent libraries:"
sudo apt-get install libncurses5-dev \
  libgnome2-dev \
  libgnomeui-dev \
  libgtk2.0-dev \
  libatk1.0-dev  \
  libbonoboui2-dev \
  libcairo2-dev  \
  libx11-dev  \
  libxpm-dev  \
  libxt-dev  \
  python-dev  \
  python3-dev  \
  ruby-dev  \
  lua5.1  \
  liblua5.1-dev  \
  libperl-dev git

cho "$PROMPT back file:$FILE_USER_BASHRC --> $FILE_USER_BASHRC.back"
sudo cp $FILE_USER_BASHRC $FILE_USER_BASHRC.back

echo "$PROMPT back file:$FILE_ROOT_BASHRC --> $FILE_ROOT_BASHRC.back"
sudo cp $FILE_ROOT_BASHRC $FILE_ROOT_BASHRC.back

echo "$PROMPT create install dir: ..."
mkdir -p $DIR_INSTALL

echo "$PROMPT into src dir: ..."
cd $DIR_SRC

echo "$PROMPT remove install file:"
sudo make uninstall

if $DISCLEAN_OPEN; then
  echo "$PROMPT clean Compile the file:"
  sudo make distclean
fi

echo "$PROMPT create config options:"
sudo ./configure --with-features=huge \
  --enable-fail-if-missing \
  --enable-multibyte \
  --enable-rubyinterp=yes \
  --enable-python3interp=yes \
  --enable-pythoninterp=yes \
  --with-python3-config-dir=$DIR_PYTHON3 \
  --with-python-config-dir=$DIR_PYTHON2 \
  --enable-perlinterp=yes \
  --enable-luainterp=yes \
  --enable-gui=gtk2 \
  --enable-cscope  \
  --prefix=$DIR_INSTALL


echo "$PROMPT start Compile vim app:"
sudo make

echo "$PROMPT start install vim app:"
sudo make install


echo "$PROMPT show vim features:"
$DIR_INSTALL/bin/vim --version

if false; then
PARAM=$(cat $FILE_ENV| grep $STRING_PATH_CONFIG)
if [ "$PARAM" = "$STRING_PATH_CONFIG" ]; then
  cat $FILE_ENV
  echo "$PROMPT config already existed!"
else
  echo $STRING_PATH_CONFIG >> $FILE_ENV
  cat $FILE_ENV
echo "$PROMPT fist config,please <sudo reboot>!"
fi
fi

FILE_CONFIG=$FILE_USER_BASHRC
PARAM=$(cat $FILE_CONFIG| grep $STRING_PATH_CONFIG)
if [ "$PARAM" = "$STRING_PATH_CONFIG" ]; then
  echo "----------------------$FILE_CONFIG----------------------------"
  cat $FILE_CONFIG
  LOG_OUT=${LOG_OUT}"\n$PROMPT already config <$FILE_CONFIG> !"
else
  echo $STRING_PATH_CONFIG >> $FILE_CONFIG
  echo "----------------------$FILE_CONFIG----------------------------"
  cat $FILE_CONFIG
  LOG_OUT=${LOG_OUT}"\n$PROMPT fist config,please < source $FILE_CONFIG >!"
fi


FILE_CONFIG=$FILE_ROOT_BASHRC
PARAM=$(cat $FILE_CONFIG| grep $STRING_PATH_CONFIG)
if [ "$PARAM" = "$STRING_PATH_CONFIG" ]; then
  echo "----------------------$FILE_CONFIG----------------------------"
  cat $FILE_CONFIG
  echo "----------------------$FILE_CONFIG----------------------------"
  LOG_OUT=${LOG_OUT}"\n$PROMPT already config <$FILE_CONFIG> !"
else
  echo $STRING_PATH_CONFIG >> $FILE_CONFIG
  echo "----------------------$FILE_CONFIG----------------------------"
  cat $FILE_CONFIG
  echo "----------------------$FILE_CONFIG----------------------------"
  LOG_OUT=${LOG_OUT}"\n$PROMPT fist config,please < source $FILE_CONFIG >!"
fi

echo $LOG_OUT

