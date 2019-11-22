
PROMPT='>>>>>>>>>>>'
USER=$(users)
# https://www.oracle.com/technetwork/cn/java/javase/downloads/jdk8-downloads-2133151-zhs.html
# jdk-8u181-linux-x64.tar.gz
# doc https://launchpadlibrarian.net/287100910/How-to-build-toolchain.pdf

# https://launchpad.net/gcc-arm-embedded/+download
# gcc-arm-none-eabi-5_4-2016q3-20160926-src.tar.bz2
PATH_TAR_JDK=/home/lvbing
FILE_NAME_JDK= gcc-arm-none-eabi-5_4-2016q3-20160926-src.tar.bz2
FILE_USER_BASHRC=/home/$USER/.bashrc
FILE_ROOT_BASHRC=/root/.bashrc

echo "$PROMPT you shell is $SHELL"

echo "$PROMPT Change software sources to Main server."
echo "$PROMPT '/etc/apt/sources.list use ‘http://old-releases.ubuntu.com' replace all 'http://*.ubuntu.com'"

echo "$PROMPT update source list."
sudo apt-get update

echo "$PROMPT install alias lib."
sudo apt-get install apt-src \
	scons \
	mingw32-runtime \
	p7zip-full \
	gawk \gzip \
	perl \
	autoconf \
	m4 \
	automake \
	libtool \
	libncurses5-dev \
	gettext \
	gperf \
	dejagnu \
	expect \
	tcl \
	autogen \
	guile-1.6 \
	flex \
	flip \
	bison \
	tofrodos \
	texinfo \
	g++ \
	gcc-multilib \
	libgmp3-dev \
	libmpfr-dev \
	debhelper \
	texlive \
	texlive-extra-utils





echo "$PROMPT back file:$FILE_USER_BASHRC --> $FILE_USER_BASHRC.back"
sudo cp $FILE_USER_BASHRC $FILE_USER_BASHRC.back

echo "$PROMPT back file:$FILE_ROOT_BASHRC --> $FILE_ROOT_BASHRC.back"
sudo cp $FILE_ROOT_BASHRC $FILE_ROOT_BASHRC.back

PATH_APP_INSTALL=/usr/local/src/${FILE_NAME_JDK%%.*}
echo "$PROMPT rm dir:$PATH_APP_INSTALL"
sudo rm -r $PATH_APP_INSTALL
echo "$PROMPT set install dir:$PATH_APP_INSTALL"
sudo mkdir $PATH_APP_INSTALL

echo "$PROMPT Decompression flle <$FILE_NAME_JDK> to <$PATH_APP_INSTALL>."
sudo tar -zxvf $PATH_TAR_JDK/$FILE_NAME_JDK  -C $PATH_APP_INSTALL

PATH_APP_INSTALL=$PATH_APP_INSTALL/$(ls -l $PATH_APP_INSTALL |awk '/^d/ {print $NF}')
echo "$PROMPT change install dir:$PATH_APP_INSTALL"

STRING_CONFIG="\nJAVA_HOME=$PATH_APP_INSTALL\nCLASSPATH=\$JAVA_HOME/lib/\nPATH=\$PATH:\$JAVA_HOME/bin\nexport JAVA_HOME PATH CLASSPATH\n"
echo "$PROMPT get config string:"
echo $STRING_CONFIG

echo "$PROMPT get referenc config string:"
STRING_REFERENCE="JAVA_HOME=$PATH_APP_INSTALL"
echo $STRING_REFERENCE

LOG_OUT=

TEMP_STRING_REFERENCE=$STRING_REFERENCE
TEMP_STRING_CONFIG=$STRING_CONFIG
TEMP_FILE_CONFIG=$FILE_USER_BASHRC
PARAM=$(cat $TEMP_FILE_CONFIG| grep "$TEMP_STRING_REFERENCE")
if [ "$PARAM" != "" ]; then
  echo "----------------------$TEMP_FILE_CONFIG----------------------------"
  cat $TEMP_FILE_CONFIG
  echo "----------------------$TEMP_FILE_CONFIG----------------------------"
  LOG_OUT=${LOG_OUT}"\n$PROMPT already config <$TEMP_FILE_CONFIG> !"
else
  echo $TEMP_STRING_CONFIG >> $TEMP_FILE_CONFIG
  echo "----------------------$TEMP_FILE_CONFIG----------------------------"
  cat $TEMP_FILE_CONFIG
  echo "----------------------$TEMP_FILE_CONFIG----------------------------"
  LOG_OUT=${LOG_OUT}"\n$PROMPT fist config,please < source $TEMP_FILE_CONFIG >!"
fi

TEMP_STRING_REFERENCE=$STRING_REFERENCE
TEMP_STRING_CONFIG=$STRING_CONFIG
TEMP_FILE_CONFIG=$FILE_ROOT_BASHRC
PARAM=$(cat $TEMP_FILE_CONFIG| grep "$TEMP_STRING_REFERENCE")
if [ "$PARAM" != "" ]; then
  echo "----------------------$TEMP_FILE_CONFIG----------------------------"
  cat $TEMP_FILE_CONFIG
  echo "----------------------$TEMP_FILE_CONFIG----------------------------"
  LOG_OUT=${LOG_OUT}"\n$PROMPT already config <$TEMP_FILE_CONFIG> !"
else
  echo $TEMP_STRING_CONFIG >> $TEMP_FILE_CONFIG
  echo "----------------------$TEMP_FILE_CONFIG----------------------------"
  cat $TEMP_FILE_CONFIG
  echo "----------------------$TEMP_FILE_CONFIG----------------------------"
  LOG_OUT=${LOG_OUT}"\n$PROMPT fist config,please < source $TEMP_FILE_CONFIG >!"
fi

echo $LOG_OUT

