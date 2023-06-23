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
# check if installed
git --version
# 配置
ssh-keygen -t rsa -b 4096 -C "email"
# get the public key
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
mkdir <repo>
cd <repo>    # enter dir
git init     # init repo
ls -al       # check all files
touch <file> # create a file 
mkdir <dir>  # create a dir
```

现在可以修改文件内容

```bash
git status          # check status      
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
git remote add origin git@github.com:xxxx/<repo>
git push -u origin master
```

更改文件内容

```bash
git add .
git commit -m "xx"
git push
```
