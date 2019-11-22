
DIR_SRC=/home/lvbing/toolchain
cd $DIR_SRC
git clone https://github.com/texane/stlink.git

# install cMake
sudo apt-get install cmake
# USB驱动ibusb
sudo apt-get install libusb-dev
# 用户空间USB编程库开发文件
sudo apt-get install libusb-1.0-0-dev
# 压缩解压软件/
sudo apt-get install p7zip mingw-w64

cd ./stlink

make

cd $(pwd)/build/Release
sudo make install DESTDIR=_install

echo "-----------------------------------------------"
echo "$(pwd)/_install/usr/local/bin
st-flash 	将二进制文件固化到 STM32 设备
st-info 	查询已连接 STLink 的 STM32 设备信息
st-util 	运行 GDB 服务与 STM32 设备进行交互
stlink-gui 	STlink 图形化工具
cmd:lsusb
cmd:st-info --prob
cmd:sudo st-flash write test.bin 0x8000000"
echo "-----------------------------------------------"
