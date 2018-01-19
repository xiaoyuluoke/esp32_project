# 在Win10的Linux子系统下搭建ESP32的开发环境 #
</br>

##  一.介绍 ##

### 1.Windows的Linux子系统 ###
</br>
&emsp;win10进行了内置工作于命令行的Linux子系统功能，WIN10用户只需要开启Linux子系统，并进行获取安装就可以在原生态的Windows系统下使用Linux系统的一系列功能，对于接触学习Linux的用户来说，无疑是一件利好。结合了Linux系统的Windows系统也使得在往往不能处理的事情一下变得简单和方便很多，比如在Windos下搭建Linux软件开发环境。比如安装和处理开源软件等等。下面就介绍在Windows的Linux子系统下搭建国产物联网芯片ESP32的软件开发环境。

### 2.ESP32WIFI 蓝牙双模芯片 ###
&emsp;ESP32 是乐鑫提供的集成 2.4 GHz Wi-Fi 和蓝牙双模的单芯片方案，采用台积电 (TSMC) 超低功耗的 40 纳米工艺，拥有最 佳的功耗性能、射频性能、稳定性、通用性和可靠性，适用于各种应用和不同功耗需求。</br>
&emsp;ESP32芯片采用Candence的IP解决方案，并不是我们熟知的ARM，X86等内核架构，现成的大多数集成IDE软件（Keil, CodeWarrior,CCS等）都不支持ESP32这种内核架构，乐鑫也没有提供相应的IDE环境，只是提供了基于xtensa-esp32-elf这种GCC工具链的SDK软件包。该软件SDK在Linux系统下搭建比较方便，(MAC系统不知情，提供在Windows系统下的软件SDK包，需要一个类Linux软件（高仿的Linux），尝试过，搭建起来比在ubantu虚拟机上还麻烦)。如果使用Linux虚拟机，就得涉及到Windows切换到Linux下种种不便利。</br>
&emsp;因此，一个兼容Linux系统和Windows操作系统优势之处的操作系统，无疑要方便许多，并会解决单种系统的种种不便。Windows的Linux子系统的出现，恰好解决这些麻烦之处。</br>
## 二.开启Windows的Linux子系统功能 ##
&emsp;本人在实际操作时参考了下面这篇文章:</br>
    [https://www.jianshu.com/p/bc38ed12da1d](https://www.jianshu.com/p/bc38ed12da1d)
#### 1.开启Linux子系统功能 ####
1.打开 控制面板</br>
2.选择 程序</br>
3.选择 启用程序和功能</br>
![](https://i.imgur.com/433eewB.png)
4.选择 启用Linux子系统 然后点确定，系统会提示需要关机重启后才能生效，关机重启电脑</br>
![](https://i.imgur.com/rPLAr2H.png)
#### 2.下载并安装Linux子系统uabntu ####
&emsp;上述提到的那篇文章已经详细的说明了如何下载安装Linux子系统，按理说，我已经没有什么可以讲的了。</br>
&emsp;但我在实际的操作中，会出现一个子系统无法下载下来等困境，一方面和网速有关，另一方面却和默认的开启了网络代理有关。阻碍了访问一些网站以及下载软件。在这种情况下，你无法打开Windows的软件商店，画风如下:</br>
![](https://i.imgur.com/6ZNqv8Q.jpg)
我解决此问题时参考的博客:</br>
[http://blog.csdn.net/u011303443/article/details/51558234](http://blog.csdn.net/u011303443/article/details/51558234)
解决办法如下:</br>
1.打开网络 进入到： 网络和Internet设置</br>
![](https://i.imgur.com/b6XdMms.png)
</br>
2.进入代理设置： 代理</br>
![](https://i.imgur.com/EQFizIc.png)
</br>
3.关闭代理：选择关闭</br>
![](https://i.imgur.com/RE2zkyi.png)
4.下载安装Linux子系统</br>
a.方法a:按下win+R，在弹出的输入框中输入bash,会弹出命令行，提示没有安装Linux子系统，输入y及进行下载安装Linux子系统</br>
b.方法b:按下win+R，输入cmd,在cmd里输入lxrun /install /y</br>
开始下载安装</br>

## 三.安装Linux常用软件及其支持包 ##
1.win+r输入bash打开Linux终端,在终端里输入</br>
apt-get install update</br>
2.win+r输入bash打开Linux终端，在终端里输入</br>
apt-get install upgrade</br>
3.上面两部操作完成后，可以进一步安装其他软件了，后面的操作影响不大，可以一次打开两三个bash终端同时进行操作</br>
apt-get install vim </br>
apt-get install gcc</br>
4.完成第三部操作后，进行安装gcc-4.8（ps:我之前第尝试时发现，直接安装gcc，获取的gcc没有g++等编译器以及相应的依赖，后面的软件根本无法安装，所以才有折中的办法，先安装个默认的gcc，在安装个gcc-4.8,获取完整的gcc工具链，最后安装个pip3，自动的把默认的gcc工具链升级到gcc-5去了，这些后面安装软件就不会存在缺乏g++等无法安装的尬地）</br>
apt-get install gcc</br>
apt-get install gcc-4.8</br>
5.安装pip3工具</br>
apt-get install python3-pip</br>
6.更新pip3工具</br>
pip3 install --upgrade pip</br>
6.安装git(多开两三个bash终端，同时执行不同命令，速度快点)</br>
apt-get install git</br>
## 四.进行ESP32的开发环境搭建 ##
参考网址:</br>
[https://esp-idf.readthedocs.io/en/latest/get-started/linux-setup.html](https://esp-idf.readthedocs.io/en/latest/get-started/linux-setup.html)
简约步骤：</br>
1.进行依赖包的安装</br>
sudo apt-get install git wget make libncurses-dev flex bison gperf python python-serial</br>
2.下载安装xtensa-esp32-elf-gcc工具链，下载下来后，直接放在d盘目录下（这样方便点，懒得cd）至于下载的32还是64位的编译器，以自己的电脑的操作系统而定，我下载安装的64位的gcc编译器</br>
3.在bash里查看文件列表</br>
cd /mnt/d/</br>
ls</br>
画风如下：</br>
![](https://i.imgur.com/WB81pZf.png)
安照下面的指令进行</br>
cd </br>
mkdir esp</br>
cd ~/esp</br>
cp /mnt/d/xtensa(别输入了，按table键自动补全) . </br> 
上面那句的完全指令如下：</br>
cp /mnt/d/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz .</br>
4.解压压缩包</br>
tar -xzf ~/Downloads/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz</br>
ls</br>
画风如下:</br>
![](https://i.imgur.com/QBzS7bx.png) 
5.设置xtensa-esp32-elf-gcc的环境变量</br>
vim ~/.profile</br>
在最后一行出添加这样一句</br>
export PATH="$PATH:$HOME/esp/xtensa-esp32-elf/bin"</br>
操作如下:</br>
vim ~/.profile</br>
![](https://i.imgur.com/wRqsti7.png)
键盘上的上下左右键可以移动光标(Ctrl+F更快些)，移动到最低处，
在bash界面右键鼠标，就把复制的内容粘贴上去了。</br>
在英文输入法下输入o进入插入模式</br>
然后粘贴这一句：</br>
export PATH="$PATH:$HOME/esp/xtensa-esp32-elf/bin"</br>
输入ESC键退出插入模式，输入</br>
:wq</br>
![](https://i.imgur.com/GWccwc5.png)
保存退出</br>
关闭bash终端，重新输入win+r，打开bash</br>
在命令行下输入</br>
xtensa-esp32-elf-gcc -v</br>
如果出现下面的画风，则xtensa-esp32-elf-gcc工具链安装成功</br>
![](https://i.imgur.com/zgtZgZW.png)
6.获取ESP-IDFSDK软件包</br>
cd ~/esp</br>
git clone --recursive https://github.com/espressif/esp-idf.git</br>
7.设置IDF_PATH环境变量</br>
vim ~/.profile</br>

在上一次添加的内容后面再添一句</br>
export IDF_PATH=~/esp/esp-idf</br>
![](https://i.imgur.com/YWrLzgK.png)
然后保存退出</br>
8.开发环境进行测试</br>
重新打开bash终端</br>
cd到hello world工程目录下</br>
cd ~/esp/esp-idf/examples/get-started/hello_world/</br>
ls</br>
![](https://i.imgur.com/wZP64Zt.png)

输入</br>
make menuconfig</br>
如果弹出个界面，并且没报错，基本上就没问题了。</br>
![](https://i.imgur.com/YiBvdSp.png)
## 4.将windows系统的串口关联到Linux子系统上来 ##
将windows系统的串口关联到Linux子系统上来后，可以直接在bash终端下，执行make flash命令进行编译后的程序进行下载到芯片上去</br>
参考文章:</br>
[https://blogs.msdn.microsoft.com/wsl/2017/04/14/serial-support-on-the-windows-subsystem-for-linux/](https://blogs.msdn.microsoft.com/wsl/2017/04/14/serial-support-on-the-windows-subsystem-for-linux/)
简约步骤:</br>
1.首先查看串口连接到了哪个COM口上</br>
windows下的com5便是linux下的ttyS5，同理com6是linux下的ttyS6(参考这一句:
&emsp;In WSL, the lxcore driver maps COM ports to Linux devices by the COM port number so /dev/ttyS<N> is tied to COM<N>.  More specifically, the mapping is on the minor number, so minor number 65 (/dev/ttyS1) is COM1, 66 (/dev/ttyS2) is COM2, and so forth.)</br>
2.关联</br>
假设串口连接到了com3执行以下命令</br>
sudo chmod 666 /dev/ttyS3</br>
stty -F /dev/ttyS3 -a</br>
stty -F /dev/ttyS3 sane 115200</br>
好了，大功告成了，可以在Win10的Linux子系统下搞事情了。</br>
ps:在对于make menuconfig的时候，选择串口端口那里，就填ttyS3(当然，如果串口连接到的是其他com口，就填相应的ttyS)</br>
最后附一张我的Linux子系统的开发环境</br>
![](https://i.imgur.com/DXNmlNI.png)
