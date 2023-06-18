---
title: "Vscode搭建c++开发环境"
date: 2021-03-03T21:41:07+08:00
categories:
  - 过程记录
tags:
  - VSCode
  - C++
math: true
hidden: false
draft: false
toc: true
---


## 配置编译器 [mingw-w64](http://mingw-w64.org/doku.php/start)

### 下载安装

去官网下载页面，选择 `MingW-W64-builds`

![Snipaste_2021-03-03_21-45-35](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2021-03-03_21-45-35.jpg)

点击 `Sourceforge` 下载

![image-20210303215247503](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303215247503.png)

下载完解压到合适位置，找到文件夹下的 `bin` 子目录

![image-20210303215606850](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303215606850.png)

双击 `bin` 目录，将上方路径复制下来

![image-20210303215719201](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303215719201.png)

### 编辑环境变量

任务栏搜索环境变量

![image-20210303215844772](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303215844772.png)

打开环境变量

![image-20210304083201078](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210304083201078.png) 

选择下方系统变量，点击 `Path` ，编辑

![image-20210303220015375](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303220015375.png)

选择新建，将刚才复制的路径输进去，一路确认退出

![image-20210303220146233](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303220146233.png)

打开终端，输入

```shell
g++ --version
gdb --version
```

![image-20210504080430194](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210504080430194.png)

出现版本号则配置成功

## 安装 [Vscode](https://code.visualstudio.com/docs/?dv=win) 编辑器

正常安装，注意一定要添加到系统 `PATH` 中

安装插件

- Chinese
- C/C++
- Code Runner

![image-20210304083549467](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210304083549467.png)

## 配置 C++ 开发环境

打开终端，输入命令创建工作目录，该目录下存放所有 `vscode` 代码，打开 `vscode` 

```powershell
mkdir projects
cd .\projects\
mkdir c++
cd .\C++\
code .
```

![image-20210303221520861](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303221520861.png)

新建一个 `cpp` 文件 `test.cpp`

![image-20210303221651739](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303221651739.png)

输入测试内容：

```c++
#include <iostream>
#include <vector>
#include <string>
using namespace std;
int main()
{
    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};
    for (const string& word : msg)
        cout << word << " ";
    cout << "\n";
}
```

点击终端 - 配置默认生成任务

![image-20210303221854588](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303221854588.png)

选择安装的 `g++` ：

![image-20210303221958182](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303221958182.png)

会自动生成 `tasks.json` 配置文件：

![image-20210303222050512](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303222050512.png)

按 `Ctrl + Shift + B` 运行：

![image-20210303222155340](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303222155340.png)

会提示失败，因为还要配置调试文件

点击运行 - 添加配置：

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303222313671.png)

选择 `C++(GDB/LLDB)`

![image-20210303222343279](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303222343279.png)

自动生成配置文件：

![image-20210504080942097](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210504080942097.png)

再次运行，即可成功

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210303222515011.png)

就可以在当前目录下愉快地用 `vscode` 写 `C++` 了！
