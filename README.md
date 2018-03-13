Little_Star
===========


[![Build Status](https://travis-ci.org/stevenchen49/Little_Star.svg?branch=master)](https://travis-ci.org/stevenchen49/Little_Star.svg?branch=master)  ![](https://img.shields.io/badge/version-v1.0-519dd9.svg) ![](https://img.shields.io/badge/language-c-orange.svg) ![](https://img.shields.io/badge/platform-raspberryPi-lightgrey.svg)


一个 Raspberry Pi 音乐工程


主要目的:

 - 使用树莓派实现乐曲弹奏
 - 通过使用 wiringPi 熟悉树莓派上的GPIO控制方式
 - 初步了解 ALSA (Advanced Linux Sound Architecture)
 - 利用 git 进行团队程序开发

---

### 准备工作

测试使用的树莓派版本为:Raspberry Pi 3 Model B；

测试使用键盘为 3x3 矩阵键盘，6个引脚，输入方式为按键扫描输入；

矩阵键盘的引脚对应情况如下:


| 键盘引脚 | 树莓派引脚  |  wPi 引脚 |
| - | - | - |
| C1 |37 | 25|
| C2 |35 | 24 |
| C3 |33 | 23 |
| R1 |40 | 29 |
| R2 |38 | 28 |
| R3 |36 | 27 |

---

### 软件库安装

该工程所需软件库：1.`wiringPi`；2.`tinyalsa`

`wiringPi` 可以从网上软件源直接下载安装：

```
$ sudo apt-get install wiringpi
```

`tinyalsa` 源代码已经包含在工程目录下，只需切换到 tinyalsa/ 目录下安装配置即可：

```
$ make 
$ sudo make install
$ sudo ldconfig
```

---

### 工程编译
在树莓派命令终端切换到该工程目录下，完成编译并执行可执行文件：

```
$ make
$ ./main
```

3x3矩阵键盘按键功能如下所示：


| 1 | 2 | 3 |
| - | - | - |
| 4 | 5 | 6 |
| 7 | ↓ | ↑ |

1-7为音阶，上下键为高八度低八度切换

---

### 技术支持

* [wiringPi Web](http://wiringpi.com/)
* [ALSA Project](http://www.alsa-project.org/main/index.php/Main_Page)
* [Tinyalsa Github](https://github.com/tinyalsa/tinyalsa)
