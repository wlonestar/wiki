---
title: "Git常用命令"
date: 2021-02-26T13:06:46+08:00
math: true
hidden: false
draft: false
toc: true
---

## 配置 git

```bash
# 查看版本号，一般用来验证是否配置成功
git --version
# 配置
ssh-keygen -t rsa -b 4096 -C "你的邮箱"
# 获取公钥
cat ~/.ssh/id_rsa.pub
```

到 `GitHub` - `settings` - `SSH and GPG keys`，点击 `New SSH key`，添加生成的SSH

回到terminal

```bash
ssh -T git@github.com
# Are you sure you want to continue connecting(yes/no)?
# 配置git
git config --global user.name {username}
git config --global user.email {email}
```

## 使用 Git 创建版本库

###  初始化

输入命令：

```bash
mkdir <项目名称>
cd <项目名称>   #进入目录
git init       #初始仓库，新建一个 .git 目录
ls -al         #查看所有文件
touch <file1>  #创建一个文件
mkdir <dir1>   #新建一个目录
```

现在可以修改文件内容

```bash
git status -sb      #文件前面有 ?? 号
git add .           #一次性将文件添加到暂存区
git status -sb      #现在文件前面 ?? 变成了 A
git commit -m "xx"  #将你 add 过的内容正式提交到本地仓库，并添加注释
git log             #查看历史变动
```

###  文件变动

```bash
git status -sb
git add .
git commit -m "xx"
```

### 本地仓库上传到 GitHub

新建一个空仓库，名称与本地目录名一致

```bash
git remote add origin git@github.com:xxxx/<仓库名>
git push -u origin master
```

更改文件内容

```bash
git add .
git commit -m "xx"
git push
```
