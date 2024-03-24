---
title: "Docker 部署多站点"
date: 2021-10-03T14:44:48+08:00
description: 服务器同一端口部署两个网站
image: https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/7e4afde0d80946dea5898e2dfa559b81.png
draft: false
toc: true
---



生命不息，折腾不止hhh。继续捣鼓我的云服务器，再过一个月就到期需要付费了，到时候直接续个一年。

说回正题，之所以要部署两个网站呢，是因为我的确有这个需求，哪个男孩子不想要好几个自己的网站呢（狗头）。

"碰巧"昨天晚上一不小心 `rm /*` 了，关键TM我在 `root` 用户下敲了这个命令，直接把 `ls` 都给整没了，还能有什么办法了，自己手残呗，直接重装系统了。刚好是在全新的系统上搭环境，敲得命令都很全，直接按照命令来写吧。



## 准备工作

### 设置密码

```shell
sudo passwd
sudo apt-get update
sudo apt-get upgrade
sudo apt-get autoremove
sudo apt-get install neofetch
neofetch
```

进系统的第一步当然是设置密码啦，然后就是老三步： `update` 、 `upgrade` 、 `autoremove`

我习惯再装个 `neofetch` 查看系统配置

![image-20211003154333790](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20211003154333790.png)

（有一说一，腾讯云的AMD一核霄龙云服务器速度比隔壁英特尔一核至强处理器快，1核1G > 1核2G）

![image-20211003154616572](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20211003154616572.png)

偏了，回来。

### 添加 `ssh` 公钥

我还会使用 `ssh` 连服务器，这样就可以免密登录了

```shell
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
```

把本机的 `id_rsa.pub` 通过 vscode 的 ssh 发送到服务器上，再执行这个命令，就可以免密登录啦

### 安装 Docker 和 Docker Compose

这个直接看官网即可， Linux 上需要分开安装，Windows 和 Mac 上不需要

```shell
sudo apt-get update
sudo apt-get install apt-transport-https ca-certificates curl gnupg lsb-release
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
echo "deb [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io
sudo curl -L "https://github.com/docker/compose/releases/download/1.29.2/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose   
docker-compose --version
```



## 网站文件准备

### 设置目录层级

因为涉及到容器化部署，最好先考虑好服务器和容器内目录的映射关系，方便维护

```shell
mkdir -p nginx/{blog,love} www/{blog,love} ssl/{blog,love}
vim nginx/blog/nginx.conf
vim nginx/love/nginx.conf
vim www/blog/index.html
vim www/love/index.html
```

这是建成的目录树，`site/` 是我后建的，先不管

```
.
├── nginx
│   ├── blog
│   │   └── nginx.conf
│   └── love
│       └── nginx.conf
├── site
│   ├── blog
│   └── love
├── ssl
│   ├── blog
│   │   ├── 1_blog_bundle.crt
│   │   └── 2_blog.key
│   └── love
│       ├── 1_love_bundle.crt
│       └── 2_love.key
└── www
    ├── blog
    │   └── index.html
    └── love
		└── index.html
```

对于 nginx 配置文件的编写，其实我也不是很懂，算是多看看别人的博客，照猫画虎的那种，只要网站能访问不就得了（bushi），另一个同理

```
// ~/nginx/blog/nginx.conf
events {
    worker_connections 1024;
}
http {
    include mime.types;
    default_type application/octet-stream;
    server {
        listen 80;
        server_name localhost;
        root /usr/share/nginx/html;
    }
    server {
        listen                    80;
        listen                    443 ssl;
        server_name               localhost;
        ssl_certificate           /etc/ssl/cert/1_blog_bundle.crt;
        ssl_certificate_key       /etc/ssl/cert/2_blog.key;
        ssl_session_timeout       5m;
        ssl_ciphers               ECDHE-RSA-AES128-GCM-SHA256:ECDHE:ECDH:AES:HIGH:!NULL:!aNULL:!MD5:!ADH:!RC4;
        ssl_protocols             TLSv1 TLSv1.1 TLSv1.2;
        ssl_prefer_server_ciphers on;
        location / {
            root /usr/share/nginx/html;
            index index.html;
        }
    }
}

// ~/nginx/love/nginx.conf
...
```

尤其注意 `include mime.types; default_type application/octet-stream;` 这两行，具体原因参考这篇博客

[nginx部署程序css正常下载，但是页面却无法正常的显示css_笨鸟不会飞-CSDN博客](https://blog.csdn.net/linzhefeng89/article/details/102789071)

配置文件写完了再随便在 `index.html` 里写点内容

```html
// www/blog/index.html
hello blog
// www/love/index.html
hello love
```

### 上传 SSL 证书

通过 vscode 直接拖到对于的目录中即可

### 启动容器

然后就可以启动容器了

```shell
sudo docker run -p 8080:80 -p 8081:443 -v $PWD/nginx/blog/nginx.conf:/etc/nginx/nginx.conf -v $PWD/www/blog:/usr/share/nginx/html -v $PWD/ssl/blog:/etc/ssl/cert --name nginx-blog -d nginx
sudo docker run -p 8082:80 -p 8083:443 -v $PWD/nginx/love/nginx.conf:/etc/nginx/nginx.conf -v $PWD/www/love:/usr/share/nginx/html -v $PWD/ssl/love:/etc/ssl/cert --name nginx-love -d nginx
sudo docker ps
```

使用 `docker ps` 命令查看当前正在运行的容器

![image-20211003161020291](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20211003161020291.png)

可以看到两个容器分别运行在本机的 `8080` 、 `8081` 、 `8082` 、 `8083` 端口，测试一下本机能不能访问

```shell
curl 127.0.0.1:8080
curl 127.0.0.1:8081
curl 127.0.0.1:8082
curl 127.0.0.1:8083
```



## 服务器配置 Nginx

服务器上的 Nginx 用来转发端口和绑定域名

### 安装 Nginx

```shell
sudo apt-get install nginx-full
```

ubuntu 20.04 上安装这个，安装完默认启动

### 写配置文件

```shell
cd /etc/nginx/sites-available/
sudo vim blog.conf
sudo vim love.conf
```

这里就不多说了，另一个同理

```
// /etc/nginx/sites-available/blog.conf
server {
    if ($host = blog.wangjialei.xyz) {
        return 301 https://$host$request_uri;
    } # managed by Certbot
    listen 80;
    server_name blog.wangjialei.xyz;
    return 301 https://$server_name$request_uri;
}
server {
    listen 80;
    listen 443 ssl;
    server_name blog.wangjialei.xyz;
    ssl_certificate /etc/letsencrypt/live/blog.wangjialei.xyz/fullchain.pem; # managed by Certbot
    ssl_certificate_key /etc/letsencrypt/live/blog.wangjialei.xyz/privkey.pem; # managed by Certbot
    ssl_session_timeout 5m;
    ssl_protocols TLSv1 TLSv1.1 TLSv1.2;
    ssl_ciphers ECDHE-RSA-AES128-GCM-SHA256:HIGH:!aNULL:!MD5:!RC4:!DHE;
    ssl_prefer_server_ciphers on;
    location / {
        index  index.html;
        proxy_pass http://127.0.0.1:8080;
    }
}

// /etc/nginx/sites-available/love.conf
...
```

可以看到这里的 SSL 证书是 Certbot 帮我们管理的，一开始写配置文件的时候要注意填上本机的路径

### 添加软连接

在 `/etc/nginx/sites-enabled/` 下添加软连接

```shell
sudo ln -s /etc/nginx/sites-available/blog.conf /etc/nginx/sites-enabled/blog.conf
sudo ln -s /etc/nginx/sites-available/love.conf /etc/nginx/sites-enabled/love.conf
```

### 测试是否配置成功

测试如果没有错误信息就可以重启本机的 Nginx 服务了

```shell
sudo nginx -t
service nginx restart
```

![image-20211003162713971](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20211003162713971.png)

## 安装 SSL 证书

先安装 Certbot，再分别配置每个域名

```shell
sudo apt-get install python3-certbot-nginx
sudo ufw allow 'Nginx Full'
sudo certbot --nginx -d blog.wangjialei.xyz   
sudo certbot --nginx -d love.wangjialei.xyz  
```



## 通过 Git 上传文件到云服务器

因为我这两个都是静态博客，只需要在本地生成打包好的文件上传服务器即可，但是每次生成再上传太麻烦。

还记得前面说过的 `site` 文件夹吗，我们可以借助 Git 的 hooks （钩子）功能实现一些自动操作

### 创建裸仓库

在服务器上创建裸仓库

```shell
cd ~/site/blog
git init --bare && cd hooks && touch post-receive
vim post-receive
chmod 777 post-receive
```

`post-receive` 脚本就是在提交后将提交的文件写入 `~/www/blog/html`，另一个同理

记得给 `post-receive` 脚本全部权限，就因为权限问题我在这卡了好一会儿

```bash
#!/bin/bash
GIT_REPO=/home/ubuntu/site/blog
TMP_GIT_CLONE=/tmp/blog
NGINX_HTML=/home/ubuntu/www/blog
rm -rf ${TMP_GIT_CLONE}
git clone $GIT_REPO $TMP_GIT_CLONE
rm -rf ${NGINX_HTML}/*
cp -rf ${TMP_GIT_CLONE}/* ${NGINX_HTML}
```

### 本地打包上传

切到要上传的目录下，创建仓库，远程到服务器上指定的位置，，另一个同理

```shell
git init
git add .
git commit -m "message"
git remote add origin user_name@your_server_ip:/home/ubuntu/site/blog
```

或许要重启服务器上的 Nginx 服务器？不是很清楚。

到这里就完成啦！这两个网址都指向服务器的 443 端口

![image-20211003165237076](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20211003165237076.png)

![image-20211003165415782](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20211003165415782.png)

因为不太了解docker的网络，有时候网站的一些路径在公网访问会被定向到 `127.0.0.1/xx` ，就是服务器本机，有点搞不懂，但是目前网站访问没什么问题，就先这样吧，再学学 Docker 和 Nginx，或许就懂了。



## 参考博客

- [给Docker中的Nginx搭建HTTPS环境 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/43375158)

- [nginx重启 failed (98: Address already in use)_zqinghai的专栏-CSDN博客](https://blog.csdn.net/zqinghai/article/details/73484394)

- [解决故障码400，“”The plain HTTP request was sent to HTTPS port“” - 戴红领巾的少年 - 博客园 (cnblogs.com)](https://www.cnblogs.com/peng-zone/p/11833320.html)

- [h5应用经过Nginx代理，css无法加载的问题 - 掘金 (juejin.cn)](https://juejin.cn/post/6987188150929981453)

- [Nginx核心要领五：worker_processes、worker_connections设置_zhuyu19911016520-CSDN博客](https://blog.csdn.net/zhuyu19911016520/article/details/90714429)

- [docker常规操作——启动、停止、重启容器实例_Michel Liu-CSDN博客_docker 停止容器](https://blog.csdn.net/Michel4Liu/article/details/80889977)

