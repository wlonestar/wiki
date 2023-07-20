---
title: "WSL2+Scoop:打造舒适开发环境"
description: 我愿称之为直接起飞
date: 2022-10-29T21:41:36+08:00
image: https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/IMG20210624185947_BURST000_COVER.jpg
categories:
  - 知识整理
tags:
  - wsl2
  - scoop
math: true
hidden: false
draft: false
---

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/wsl-1.jpg)

## 配置 WSL

适用于 Linux 的 Windows 子系统可让开发人员按原样运行 GNU/Linux 环境 - 包括大多数命令行工具、实用工具和应用程序 - 且不会产生传统虚拟机或双启动设置开销。

### 先决条件

1. 启用适用于 Linux 的 Windows 子系统

```powershell
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
```

2. 启用虚拟机功能 

```powershell
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
```

3. 下载 Linux 内核更新包

这里是 [下载链接](https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi)


3. 设置默认版本为 wsl2

```powershell
wsl --set-default-version 2
```

### 下载

[官方文档](https://learn.microsoft.com/zh-cn/windows/wsl/install-manual)

手动下载特定发行版并移动到目标位置，wsl 就不会按默认安装到 C 盘

Ubuntu 22.04 下载链接：[Ubuntu22.04](https://aka.ms/wslubuntu2204)

将安装包后缀名改成 `zip`，手动解包，取出需要的安装包

### 安装

上一步得到的安装文件夹如下图所示

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2022-10-29_21-59-28.png)

双击 `ubuntu2204.exe` 即开始安装，填入相关信息即可

### 配置

1. 更换软件源

这里推荐 [ustc](https://mirrors.ustc.edu.cn/help/ubuntu.html) 镜像源

```
# ustc mirrors
deb https://mirrors.ustc.edu.cn/ubuntu/ jammy main restricted universe multiverse
deb https://mirrors.ustc.edu.cn/ubuntu/ jammy-security main restricted universe multiverse
deb https://mirrors.ustc.edu.cn/ubuntu/ jammy-updates main restricted universe multiverse
deb https://mirrors.ustc.edu.cn/ubuntu/ jammy-backports main restricted universe multiverse
```

2. 安装必要工具

```sh
sudo apt-get install vim wget curl net-tools neofetch zsh tree baobab neovim -y
```

3. 配置 vim 和 neovim

> 这部分经常更新，如有需要建议移步我的neovim配置仓库：[wlonestar/nvim.lua: my neovim configuration](https://github.com/wlonestar/nvim.lua)

```
" ~/.vimrc
syntax on
filetype on
setlocal noswapfile
set bufhidden=hide
set nu
set ruler
set showmatch
set cindent
set tabstop=2
set autoindent
set smartindent
set cursorline
set shiftwidth=2
set softtabstop=2
set nobackup
set autochdir
set magic
set matchtime=2
set backspace=indent,eol,start
set cmdheight=1
set laststatus=2
set cc=80
```

```
" ~/.config/nvim/init.vim
set nobackup
set noswapfile
set cursorline
set cc=80
set number
set tabstop=2
set softtabstop=2
set expandtab
set shiftwidth=2
set autoindent
set showmatch
set ignorecase
set hlsearch
set incsearch
set wildmode=longest,list
set mouse=a
set mouse=v
set cmdheight=1
set laststatus=2
set matchtime=2
set title
set wildmenu
syntax on
filetype plugin indent on
let g:netrw_banner=0
```

4. 配置 `git`

这里很重要，配置好 `ssh` 后 clone 仓库会更快

```
[credential]
  helper = manager-core
[user]
  name = wlonestar
  email = wlonestar@163.com
```

5. 配置 `zsh`

下载 `ohmyzsh`：

```sh
git clone git@github.com:ohmyzsh/ohmyzsh.git ~/ohmyzsh
sh ~/ohmyzsh/tools/install.sh
```

安装 zsh 插件：

```sh
git clone https://github.com/zsh-users/zsh-autosuggestions ~/.oh-my-zsh/plugins/zsh-autosuggestions
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ~/.oh-my-zsh/plugins/zsh-syntax-highlighting
```

修改 `~/.zshrc`

```
plugins=(
  git
  zsh-autosuggestions
  zsh-syntax-highlighting
)
```

```sh
source ~/.zshrc
```

6. 安装相关开发环境

将开发环境安装在 wsl ，Windows 上安装相关客户端软件或者 IDE

```sh
# python3
sudo apt-get install python3-pip -y

# c/c++
sudo apt-get install build-essential clang gdb cmake -y

# java
sudo apt-get install openjdk-17-jdk -y

# rust
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

## 配置 Scoop

Scoop: A command-line installer for Windows

Scoop 能让程序员留在命令行上，提高效率，而且扩展了 PowerShell，可以使用能够很好处理文本的程序。

### 设置安装路径

scoop 会安装到其默认位置：`C:\Users\<YOUR USERNAME>\scoop`

要想更改路径，可以预先设置环境变量

```
SCOOP         -> D:\scoop
SCOOP_GLOBAL  -> D:\scoop
```

### 安装

执行命令

```powershell
Set-ExecutionPolicy RemoteSigned -Scope CurrentUser # Optional: Needed to run a remote script the first time

irm get.scoop.sh | iex
```

### 下载必要工具

1. 下载 git，开发需要，Windows 环境也需要安装 git

```sh
scoop install git -g
```

按 wsl 设置好 git

```
[credential]
  helper = manager-core
[user]
  name = wlonestar
  email = wlonestar@163.com
```

2. 安装 sudo，像 Unix 系统一样提升用户权限

```sh
scoop bucket add main
scoop install sudo -g
```

3. 安装其他应用程序

借助 scoop 的导出命令，导出到文件中， 见 [wlonestar/scoopfile.json](https://gist.github.com/wlonestar/52951b9376dac9a6d712c7440f4de403)

- 完 -

