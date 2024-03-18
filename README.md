简单时钟显示
===
简介
----
本项目所用单片机和屏幕都是来自合宙的 air001 与 air10x屏幕 air001 arm Comtex-m0内核架构，内置集成32kB的字节寻址flash以及4kB ram，工作最高主频48Mhz，内置rc振荡器也支持外部晶振时钟
屏幕尺寸为0.96 分辨率为160*80 通信协议spi 驱动库st7735 
其中数字与汉字的显示通过取模软件进行取模后然后通过异或运算输出单个像素点形成图像
#引脚接口
由于air001 与air10x的屏幕显示引脚孔位部分合适能够直接进行对插就不再说明，air10x上自带控制按钮但是部分孔位无法使用经过测试可使用的引脚有
PF00 PF01 PB03 (其中PA14引脚可复用为GPIO,但是烧录时通过swd烧录就没有复用该引脚，复用该引脚后可通过ttl串口烧录恢复)
保留的引脚有3个分别用于 选着时间 调整时间 确定 3项功能。
该项目不定期更新！
![image](https://github.com/Mieir/air001-lcdair10x-/blob/main/picture/9bbabb760bb99b871abf8afe679e4d8.jpg)
![image](https://github.com/Mieir/air001-lcdair10x-/blob/main/picture/640.png)
![image](https://github.com/Mieir/air001-lcdair10x-/blob/main/picture/afec5f0d79ee888f91b70405666e188.jpg)
![image](https://github.com/Mieir/air001-lcdair10x-/blob/main/picture/lcd_air101_image.png)