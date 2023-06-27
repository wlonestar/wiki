---
title: "基于WordPress搭建电商网站"
date: 2023-06-27T20:37:05+08:00
categories:
  - 知识整理
tags:
  - Docker
  - Nginx
  - WordPress
math: true
hidden: false
draft: false
toc: true
---

<!-- cover -->

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/c56860f9797048c5a8b4d335834bfbfc.jpg)

---

<!-- main content -->

## 网站环境配置

### 注册域名

这里使用之前已经注册好的一个域名，.top 域名相对便宜很多，个人站很合适。

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/linux_1.jpg)

### 购买云服务器

我们选择的是一台腾讯云在中国香港地区的2核4G的云服务器，
选择在香港地区可以免去网站备案的手续，
因为仅供课程设计使用，付费方式选择按量付费。

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/linux_2.jpg)

### 远程登录云服务器

使用腾讯云提供的 orcaterm 远程登录云服务器。

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/linux_3.jpg)

### 安装 Docker

按照 Docker 官网步骤安装 Docker Engine 和 Docker Compose。

1. 设置存储库：

（1）更新apt包索引和安装包，允许 apt 使用一个通过 HTTPS 的存储库： 

```bash
sudo apt-get update
sudo apt-get install ca-certificates curl gnupg
```

（2）添加 Docker 的官方 GPG 密钥： 

```bash
sudo install -m 0755 -d /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
sudo chmod a+r /etc/apt/keyrings/docker.gpg
```

（3）设置存储库：

```bash
echo "deb [arch="$(dpkg --print-architecture)" signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu "$(. /etc/os-release && echo "$VERSION_CODENAME")" stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
```

2. 安装 Docker 引擎：

（1）更新 apt 包索引:

```bash
sudo apt-get update
```

（2）安装 Docker Engine、containerd 和 Docker Compose：

```bash
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-compose-plugin
```

（3）创建 hello-world 容器验证 Docker Engine 安装是否成功：

```bash
sudo docker run hello-world
```

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/linux_4.jpg)

### 配置 Git

在 GitHub 创建一个私有仓库，并通过加入合作者的方式协同开发。

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/linux_10.jpg)

1. 生成 SSH 密钥：

```bash
ssh-keygen -t rsa -b 4096 -C "email"
cat ~/.ssh/id_rsa.pub
```

2. 在 GitHub 添加公钥：
到 GitHub - Settings - SSH and GPG keys，选择 New SSH key，添加生成的 SSH

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/linux_11.jpg)

3. 配置 Git：

```bash
ssh -T git@github.com
git config --global user.name {username}
git config --global user.email {email}
```

### 使用 Docker 和 Nginx 配置网站

从 GitHub 克隆仓库到云服务器，
仓库的主要内容包含 **docker-compose.yml** 配置文件以及 **nginx** 的配置文件。

1. MySQL 服务：
将数据库文件挂载到 **db_data** 数据卷，并复制到当前目录下的 **db** 目录下，便于迁移。

```yml
  mariadb:
    image: mariadb
    restart: always
    environment:
      ...
    volumes:
      - db_data:/var/lib/mysql
      - ./db:/var/lib/mysql
```

2. WordPress 服务：
使用 **wordpress:php8.1-apache** 镜像安装 WordPress。
将Wordpress 数据挂载到数据卷，并复制到当前目录下的 **wordpress** 目录下，便于迁移站点。

```yml
  wordpress:
    image: wordpress:php8.1-apache
    restart: always
    stdin_open: true
    tty: true
    environment:
      ...
    volumes:
      - wordpress_data:/var/www/html
      - ./wordpress:/var/www/html
```

3. certbot服务：
我们选择使用免费的 Let's Encrypt 证书，利用 **certbot** 镜像安装。
将生成的SSL证书文件复制到当前目录下的 certbot 目录下，为后续 Nginx 配置证书做准备。

```yml
  certbot:
    image: certbot/certbot:latest
    command: ...
    volumes:
      - ./certbot/conf:/etc/letsencrypt
      - ./certbot/logs:/var/log/letsencrypt
      - ./certbot/data:/var/www/html
```

4. Nginx 服务：
创建 **nginx/conf** 目录，在该目录下创建 **default.conf** 配置文件，
编写 nginx 配置文件，设置 **server_name** 为注册的域名。

```conf
server {
  listen [::]:80;
  listen 80;
  server_name okito.top;
  return 301 https://okito.top$request_uri;
}
server {
  listen [::]:443 ssl http2;
  listen 443 ssl http2;
  server_name okito.top;
  ssl_certificate /etc/nginx/ssl/live/okito.top/fullchain.pem;
  ssl_certificate_key /etc/nginx/ssl/live/okito.top/privkey.pem;
  root /var/www/html;
  index index.php;
  location ~ /.well-known/acme-challenge {
    allow all;
    root /var/www/html;
  }
  location / {
    try_files $uri @apache;
  }
  location ~ ^/.user.ini {
    deny all;
  }
  location ~* .(svg|svgz)$ {
    types {}
    default_type image/svg+xml;
  }
  location = /favicon.ico {
    log_not_found off;
    access_log off;
  }
  location = /robots.txt {
    allow all;
    log_not_found off;
    access_log off;
  }
  location @apache {
    proxy_set_header X-Real-IP  $remote_addr;
    proxy_set_header X-Forwarded-For $remote_addr;
    proxy_set_header X-Forwarded-Proto $scheme;
    proxy_set_header Host $host;
    proxy_pass http://wordpress:80;
  }
  location ~[^?]*/$ {
    proxy_set_header X-Real-IP  $remote_addr;
    proxy_set_header X-Forwarded-For $remote_addr;
    proxy_set_header X-Forwarded-Proto $scheme;
    proxy_set_header Host $host;
    proxy_pass http://wordpress:80;
  }
  location ~ .php$ {
    proxy_set_header X-Real-IP  $remote_addr;
    proxy_set_header X-Forwarded-For $remote_addr;
    proxy_set_header X-Forwarded-Proto $scheme;
    proxy_set_header Host $host;
    proxy_pass http://wordpress:80;
  }
}
``` 

编写 nginx 服务，使用数据卷技术，将写好的配置文件复制到 nginx 容器内的目录下，
将 certbot 容器生成的 SSL 证书文件复制到 nginx 容器内的目录下。
 
```yml
  nginx:
    image: nginx:latest
    restart: unless-stopped
    ports:
      - 80:80
      - 443:443
    volumes:
      - ./nginx/conf:/etc/nginx/conf.d
      - ./certbot/conf:/etc/nginx/ssl
      - ./certbot/data:/var/www/html
```

完整的 **docker-compose.yml** 文件如下：

```yml
services:
  wordpress:
    container_name: wordpress
    image: wordpress:php8.1-apache
    restart: always
    stdin_open: true
    tty: true
    environment:
      WORDPRESS_DB_HOST: mariadb
      WORDPRESS_DB_USER: db_user
      WORDPRESS_DB_PASSWORD: db_user_pass
      WORDPRESS_DB_NAME: db_name
    volumes:
      - wordpress_data:/var/www/html
      - ./wordpress:/var/www/html
  mariadb:
    container_name: mariadb
    image: mariadb
    restart: always
    environment:
      MYSQL_DATABASE: db_name
      MYSQL_USER: db_user
      MYSQL_PASSWORD: db_user_pass
      MYSQL_RANDOM_ROOT_PASSWORD: 'root_pass'
    volumes:
      - db_data:/var/lib/mysql
      - ./db:/var/lib/mysql
  nginx:
    container_name: nginx
    image: nginx:latest
    restart: unless-stopped
    ports:
      - 80:80
      - 443:443
    volumes:
      - ./nginx/conf:/etc/nginx/conf.d
      - ./certbot/conf:/etc/nginx/ssl
      - ./certbot/data:/var/www/html
  certbot:
    container_name: certbot
    image: certbot/certbot:latest
    command: certonly --webroot --webroot-path=/var/www/html --email wlonestar@163.com --agree-tos --no-eff-email -d okito.top
    volumes:
      - ./certbot/conf:/etc/letsencrypt
      - ./certbot/logs:/var/log/letsencrypt
      - ./certbot/data:/var/www/html
volumes:
  db_data:
  wordpress_data:
```

1. 添加域名解析：
将注册的域名解析到购买的云服务器的公网 IP，如下所示。

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/linux_6.jpg)

6. 启动容器：
使用以下命令启动容器，开启各项服务。

```bash
sudo docker compose up -d
```

查看运行的容器：
certbot 容器申请完 SSL 证书就停止工作了，这里看不到属于正常现象。

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/linux_5.jpg)

此时访问域名即可访问网站，图为配置好的网站。

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/linux_7.jpg)


## 电商平台后台管理界面

后台管理界面如下，外观主题为Elementor，插件使用的是WooCommerce。

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/linux_admin_1.png)

Elementor外观主题为我们提供了一个简单而强大的工具，
我们用户能够以直观的方式创建定制化的WordPress网站，无需编码知识。
我们完全可以通过拖放操作和预设计模板来构建吸引人的页面，
并使用丰富的主题设置选项进行个性化定制。

WooCommerce是一个功能强大且灵活的电子商务插件，
为WordPress用户提供了建立和管理在线商店的完整解决方案。
无论是小型企业还是大型电子商务网站，
WooCommerce都为用户提供了一个可靠的平台来展示和销售他们的产品和服务。

这里不过多介绍了。

<!-- end -->

---

以上。
