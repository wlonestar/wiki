---
title: "okito项目后续开发计划"
description: okito 项目简单介绍及后续开发计划
date: 2023-02-27T08:02:35+08:00
image: https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_10-28-29.jpg
categories:
  - 开源项目
tags:
  - Spring Boot
  - React
math: true
hidden: false
draft: false
---


## 简介

okito 是一个计算机方面的专业技术内容分享交流平台，具有优质的内容和较好的搜索机制。

目标是软件简约且轻量，具有现代的界面和良好的交互体验。

项目采用C-S/B-S架构，覆盖网页端和移动端。

- 服务端采用 Spring Boot 3.x 及相关组件构建，数据库选择 PostgreSQL

- 网页端计划采用 React 构建

- 移动端计划使用 React Native 进行跨平台开发

## 目前效果

![首页](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_15-30-49.jpg)

![想法页](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_15-30-58.jpg)

![更改主题](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_15-31-08.jpg)

![登录页](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_15-31-14.jpg)

![注册页](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_15-31-19.jpg)

![登录后的主页](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_15-31-57.jpg)

![个人主页](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_15-32-12.jpg)

![关注的内容](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_15-32-28.jpg)

![标签下的文章](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_15-32-42.jpg)

![文章页](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_15-33-04.jpg)

![markdown样式](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_15-33-33.jpg)

![文章评论列表](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2023-02-27_15-33-51.jpg)


## 待开发任务

### 服务端

1. 认证模块

- [ ] 用户注册向注册邮箱发送验证码

- [ ] 用户登录忘记密码可以通过邮箱找回

- [ ] 用户登录直接使用邮箱加验证码登录(?)

- [ ] 添加更加细化的角色权限设置(?)

- [ ] 系统通知功能

2. 推荐模块

- [x] 文章及想法随机选取数据库前100(<=100)数据

3. 文章及想法模块

- [x] 查询用户关注的文章及想法

4. 搜索模块

- [x] 对文章、用户等进行搜索

5. 审核模块

- [ ] 文章实体添加字段：审核状态，未经审核的文章不公开

- [ ] 各种内容的举报及删除

- [ ] 级联删除功能

6. 其他根据前端需求添加的功能点

### 网页端

1. 主页和想法页面

- [ ] 文章按分类列表?

- [x] 登录后查询关注的用户发布的文章及想法

2. 文章页面

- [x] 文章页面markdown样式修改

- [ ] 文章目录样式修改

- [x] 文章页面收藏功能

- [x] 收藏夹对话框

3. 标签页面

- [x] 标签页面样式修改

4. 专栏页面

- [x] 专栏页面的操作、样式修改、文章列表的排序

5. 收藏夹页面

- [x] 收藏夹页面文章列表

6. 个人主页

- [x] 编辑个人信息功能

- [ ] 更换个人头像功能

7. 创作中心

- [ ] 导航栏设计

- [ ] 内容操作

- [ ] 数据展示

### 管理后台

- [ ] 需要重构 

### 移动端

尚未开始

- [ ] 页面原型设计
