---
title: "Pytorch模型部署"
date: 2022-03-10T16:54:04+08:00
math: true
hidden: false
draft: false
toc: true
---


## 服务器基本配置

### ssh 免密登录

```bash
# open authorized_keys
vim ~/.ssh/authorized_keys
# copy local public key to it
```

### conda 下载安装

```bash
# install miniconda
bash Miniconda3-py39_4.11.0-Linux-x86_64.sh
# init conda
conda init
```

## pytorch 环境配置

```bash
# create env
conda create -n <env name> python=3.8
# env
conda activate <env name>
# flask & request
pip install flask flask-restful requests
# pytorch (no gpu)
pip3 install torch==1.10.2+cpu torchvision==0.11.3+cpu torchaudio==0.10.2+cpu -f https://download.pytorch.org/whl/cpu/torch_stable.html
# gunicorn
pip install gunicorn
```

## 服务部署

```bash
# cd to the app folder
gunicorn -D -w 4 -b 0.0.0.0:5000 app:app
```

kill task

```bash
kill -s 9 <pid>
```

check task

```bash
ps -aux
```
