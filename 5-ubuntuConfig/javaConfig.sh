
PROMPT='>>>>>>>>>>>'
# https://www.oracle.com/technetwork/cn/java/javase/downloads/jdk8-downloads-2133151-zhs.html
# jdk-8u181-linux-x64.tar.gz

PATH_TAR_JDK=/home/lvbing
FILE_NAME_JDK=jdk-8u181-linux-x64.tar.gz
FILE_CONFIG=/etc/profile


PATCH_APP_INSTALL=/usr/local/src/${FILE_NAME_JDK%%.*}
echo "$PROMPT set install dir:$PATCH_APP_INSTALL"
sudo rm -r $PATCH_APP_INSTALL
sudo mkdir $PATCH_APP_INSTALL

echo "$PROMPT Decompression flle <$FILE_NAME_JDK> to <$PATCH_APP_INSTALL>."
sudo tar -zxvf $PATH_TAR_JDK/$FILE_NAME_JDK  -C $PATCH_APP_INSTALL

PATCH_APP_INSTALL=$PATCH_APP_INSTALL/$(ls -l $PATCH_APP_INSTALL |awk '/^d/ {print $NF}')
echo "$PROMPT change install dir:$PATCH_APP_INSTALL"

STRING_CONFIG="JAVA_HOME=$PATCH_APP_INSTALL\nCLASSPATH=\$JAVA_HOME/lib/\nPATH=\$PATH:\$JAVA_HOME/bin\nexport JAVA_HOME PATH CLASSPATH\n"
echo "$PROMPT get config string:"
echo $STRING_CONFIG

echo "$PROMPT get config string referenc:"
STRING_REFERENCE="JAVA_HOME=$PATCH_APP_INSTALL"
echo $STRING_REFERENCE

LOG_OUT=
PARAM=$(cat $FILE_CONFIG | grep "$STRING_REFERENCE")
echo "$PROMPT get grep results:"
echo $PARAM

if [ "$PARAM" != "" ]; then
  echo "----------------------$FILE_CONFIG----------------------------"
  cat $FILE_CONFIG
  echo "----------------------$FILE_CONFIG----------------------------"
  LOG_OUT=${LOG_OUT}"\n$PROMPT already config <$FILE_CONFIG> !"
else
  echo $STRING_CONFIG >> $FILE_CONFIG
  echo "----------------------$FILE_CONFIG----------------------------"
  cat $FILE_CONFIG
  echo "----------------------$FILE_CONFIG----------------------------"
  LOG_OUT=${LOG_OUT}"\n$PROMPT fist config,please :source $FILE_CONFIG"
fi

echo $LOG_OUT




