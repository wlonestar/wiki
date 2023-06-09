---
title: "基于GitHub和腾讯云的个人主页搭建"
date: 2023-04-11T16:18:56+08:00
categories:
  - 知识整理
tags:
  - Github
  - 个人主页
  - 图床
math: true
hidden: false
draft: false
toc: true
---

## 介绍

本文基于 GitHub Pages 和腾讯云对象存储搭建一个高度可定制的个人主页或个人博客

Overview:

- 静态网站生成

- 网页部署

- 对象存储

注意：

- 运行在 linux 环境下

- 需要熟悉 Git 和 GitHub 基本操作

## 为什么需要个人主页/个人博客

> 个人网站是指由个人或团体根据自己的兴趣爱好或价值取向，为了展示自我、与人交流，以非盈利为目的而在网络上创建的供其他人浏览的网站。就内容而言，此时期个人网站所提供的，多半是纯文字或图片等静态信息。

我们的看法：

- 个人主页展示自己的能力，甚至作为自己的简历

  - 做过的工作、感兴趣的研究方向等

- 撰写博客是一种深入理解所学知识的方式，以教代学，让输出倒逼输入

- 自由度高，不受制于平台内容审核监管制度

业内有名的个人网站

- [廖雪峰的官方网站](https://www.liaoxuefeng.com/)

- [阮一峰的网络日志](https://www.ruanyifeng.com/blog/)

- ...

我翻阅过的一些个人网站

- [SAquariusの梦想屋](https://blog.songjiahao.com/)

- [DIYgod](https://diygod.me/)

- ...

还有我的博客网站

- [lonestar's blog](https://blog.wangjialei.xyz/)

## 静态网站生成器：Hugo

### “世界上最快的网站构建框架”

> [Hugo](https://gohugo.io/) 是一个用 Go 编写的快速而现代的静态网站生成器，旨在让网站创建再次变得有趣。

特点：

- 一个静态站点生成器

- 构建网站非常快速

- 可以托管在任何地方，并且可以很好地与CDN配合使用

- 无需数据库或依赖于Ruby，Python等昂贵的运行时即可运行

### 安装和入门

- Hugo 安装仅需一个可执行文件 [hugo v0.111.3](https://github.com/gohugoio/hugo/releases/tag/v0.111.3)

- 我们提供了一个方便的 shell 脚本快速 [quick.sh](https://wangjialei.xyz/personal_site/quick.sh)

```bash
# 创建名为 <my-blog> 的站点，对应于本地一个文件夹
hugo new site my-blog && cd my-blog
git init # 初始化git仓库 
# 添加主题
git submodule add \
  git@github.com:hugo-sid/hugo-blog-awesome.git \
  themes/hugo-blog-awesome
# 写入配置文件
echo "theme = 'hugo-blog-awesome'" >> config.toml 
# 复制示例站点中的博客 
cp themes/hugo-blog-awesome/exampleSite/content/* -r content
# 启用本地服务器
hugo server
```

### Hugo 目录结构

```bash
my-blog/
├── archetypes/ # 预配置模板，用于统一站点文章元数据
│   └── default.md
├── assets/
├── content/ # 网站的所有内容
├── data/
├── layouts/ # 网页模板文件
├── public/ # 构建好的静态网页
├── static/ # 静态内容：图像,css,js等
├── themes/ # 主题
└── config.toml # 配置文件
```

- 修改主题样式/自己编写一套主题，官网有详细文档 
  
  - [Introduction to Hugo Templating](https://gohugo.io/templates/introduction/)

- 拿来即用，[Hugo Themes](https://themes.gohugo.io/) 也有很多用户上传的主题

  - 需按照提供的文档配置

- 有没有简单部署网站的方法呢？试试 [Github Pages](https://pages.github.com/)

## 随时访问：GitHub Pages

### 年轻人的第一个域名

> 直接从您的 GitHub 存储库托管。只需编辑、推送，您的更改即可生效。

无需云服务器即可部署静态网站

- 一个 GitHub 账号

- 创建一个名为 `<github_username>.github.io` 的仓库

- 创建`index.html`文件，直接写入 `Hello, world`

- 提交更改，稍作等待即部署成功

  - `https://<github_username>.github.io`

- 对于较为复杂的任务，比如我们使用的 Hugo 框架，使用`hugo server`命令生成静态文件再部署，就需要使用到 GitHub Actions 了。

### GitHub Actions

一种持续构建和持续交付（CI/CD）平台，可用于自动生成、测试和部署。

比如：

- 部署一个静态站点

- 对合并请求的代码进行测试和评估

- 自动化任务

- 发布一个开源项目，Java的jar包，node的npm包

- ...

### 简要介绍

- **工作流**：一个可配置的自动化过程，运行一个或多个作业，由 yaml 配置文件定义

- **事件**：触发工作流程运行的特定活动，类似数据库中的触发器

  - push 后执行、定时执行

- **作业**：工作流在同一运行器上执行的一组步骤，步骤按顺序执行，相互依赖

  - 步骤：shell脚本、某个动作等

- **操作**：用于 GitHub Actions 平台的自定义应用程序，执行复杂但经常重复的任务

  - 设置正确的工具链等

- **运行器**：触发工作流时运行工作流的服务器

  - Ubuntu Linux、Microsoft Windows or MacOS

### 一个自动化爬虫的例子

项目地址：[wlonestar/HHU_URP_auto_login](https://github.com/wlonestar/HHU_URP_auto_login) 配置文件 [sendmail.yml](https://wangjialei.xyz/personal_site/sendmail.yml)

免去每次登录教务系统输入密码和验证码的麻烦，解放双手！

- 每天 22:00 运行 py 脚本，从学校教务系统爬取信息
- 使用HTML模板生成邮件，发送到指定的邮箱

```yaml
name: send mail # 名称
on: # 触发器
  schedule:
    - cron: "00 14 * * *" # 每天 UTC+8 22:00 
jobs: # 工作流
  build:
    runs-on: ubuntu-latest # 运行器
    steps: # 步骤
    - name: checkout repo content
      uses: actions/checkout@v2
    - name: setup python # 设置python版本
      uses: actions/setup-python@v2
      with:
        python-version: 3.9
    - name: install dependencies # 安装 pip 依赖
      run: |
        pip install -r requirement.txt
    - name: execute py script # 执行自定义 py 脚本
      run: |
        python main.py -u $Username -p $Password
      env:
        Username: ${{ secrets.HHU_USERNAME }} # 学号
        Password: ${{ secrets.HHU_PASSWORD }} # 密码
    - name: send email # 发送邮件
      uses: dawidd6/action-send-mail@v2
      with:
        server_address: smtp.qcloudmail.com
        server_port: 465
        username: ${{ secrets.MAIL_USERNAME }} # 发件邮箱
        password: ${{ secrets.MAIL_PASSWORD }} # 发件邮箱密码
        subject: 空闲教室情况
        body: file://template/result.html
        to: ${{ secrets.EMAIL }} # 目标邮箱
        from: GitHub Actions
        content_type: text/html
```

### 使用现成的模板

官方提供了 Hugo 部署静态网站的模板

- 先关联本地仓库和 GitHub 上的仓库

```bash
git remote add origin \
  git@github.com:wlonestar/wlonestar.github.io.git
git push -u origin master
```

- `Settings-Pages` 页面选择 GitHub Actions 构建，Hugo 模板

- 稍作等待，即可访问

我们准备了一个方便的 [Makefile](https://wangjialei.xyz/personal_site/Makefile)

- 博客中的图片呢？

## 图片怎么办：对象存储

### 对象存储

> 对象存储也称为基于对象的存储，是一种计算机数据存储架构，旨在处理大量非结构化数据。 与其他架构不同，它将数据指定为不同的单元，并捆绑元数据和唯一标识符，用于查找和访问每个数据单元。 这些单元（或对象）可以存储在本地，但通常存储在云端，以便于从任何地方轻松访问数据。

- 通常是照片、音频、视频等

- 需要从任何地方都能访问

- 关系数据库中若是表的属性是图片，可将图片文件对象存储，数据库中存放图片的URL

### 花上一点小钱钱，用上对象存储服务

[腾讯云对象存储COS](https://buy.cloud.tencent.com/cos)

- 上传文件都得打开网页？

使用图床工具 [PicGo](https://picgo.github.io/PicGo-Doc/zh/guide/)

- 用于快速上传图片并获取图片 URL 链接的工具

  - 支持很多平台：腾讯云、阿里云、七牛图床等

以腾讯云为例

- 设置API访问密钥管理

- 根据购买的对象存储服务设置存储区域和存储桶

<img alt="" class="center" width="300px" src="https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-04-11_20-41-50.jpg">

### 尝试一下

- 上传一张图片并获取URL

- 复制到文章中，提交更改，等待 GitHub 部署

- 访问网站查看效果

<img alt="" class="center" width="400px" src="https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-04-12_16-23-33.jpg"/>

## 总结

本次分享的内容：

- 使用 GitHub Pages 部署静态网站

- 使用对象存储服务存储图片

心得体会：

- 现有的云服务使构建部署个人网站很轻松

- 选择合适的工具可以很大程度上解放双手

- 个人网站第一位应该是内容，然后才是样式

-- 完 --
