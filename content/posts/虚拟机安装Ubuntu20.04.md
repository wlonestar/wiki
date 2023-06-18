---
title: "虚拟机安装Ubuntu20.04"
date: 2021-02-25T21:41:52+08:00
categories:
  - 学习笔记
tags:
  - Linux
math: true
hidden: false
draft: false
toc: true
---

## 前提

- Ubuntu20.04 系统镜像 [ubuntu20.04.2.0LTS](https://ubuntu.com/download/desktop)

- VMware workstation 虚拟机 [VMware Workstation 16 Pro](https://www.vmware.com/products/workstation-pro/workstation-pro-evaluation.html)

  知乎白嫖来的密钥：

  ```ZF3R0-FHED2-M80TY-8QYGC-NPKYF```

  ```YF390-0HF8P-M81RQ-2DXQE-M2UT6```

  ```ZF71R-DMX85-08DQY-8YMNC-PPHV8```

## 安装

### 新建虚拟机

打开 VMware Workstation，选择创建新的虚拟机

![vm](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226125831.jpg)

选择自定义

![vm2](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226125843.jpg)

直接下一步

![vm3](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226125855.jpg)

选择下载好的 Ubuntu20.04 镜像

![vm4](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226125904.jpg)

添加用户名和密码

![vm5](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226125914.jpg)

设置虚拟机名称和安装位置

![vm6](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226125928.jpg)

设置处理器和内存大小

![vm7](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226125935.jpg)

![vm8](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226125943.jpg)

使用网络地址转换

![vm9](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226130022.jpg)

接下来几步默认设置即可

![vm10](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226130039.jpg)

![vm11](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226130039.jpg)

![vm12](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226130049.jpg)

设定磁盘大小，不必分配所有空间

![vm13](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226130056.jpg)

设置虚拟磁盘位置

![vm14](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226130102.jpg)

可以继续修改虚拟机配置，创建后开启虚拟机

![VM15](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226130110.jpg)

等待自动安装

![vm16](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226130117.jpg)

### 配置系统

#### 更换软件源

打开 Software & Updates ，选择 ustc 或 tsinghua 镜像源

![ub1](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226130146.jpg)

#### 设置时区

打开 Settings-Date & Time，设置 Time Zone 为 Shanghai

![ub2](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226130154.jpg)

#### 设置 root 用户密码

```shell
sudo passwd
```

![ub3](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/20210226130200.jpg)

#### 安装必要工具

```shell
# net-tools
sudo apt-get install net-tools
# curl
sudo apt-get install curl
# snap
sudo apt-get install snap
# vim
sudo apt-get install vim
```

#### 安装其他开发软件

##### Ubuntu Software

- konsole
- vscode
- chromium

##### apt / snap

```bash
# g++
sudo apt-get install g++
# jdk
sudo apt-get install openjdk-14-jdk-headless
# git
sudo apt-get install git
```

至于中文输入法什么的不是必需的，可以需要的时候再安装。

整体语言设置可以调整为中文，但没必要，在英文环境下编程还是比较合适的。

可以在 Ubuntu Software 里把预装的 Libre Office 和几个游戏卸载。

VMware 下新建虚拟机默认自动安装 vmware-tools ，可以在宿主机和虚拟机间共享剪贴板和互相拖拽文件，很方便。

到这里 Linux 系统就安装完成了，顺便安装了一些开发工具

