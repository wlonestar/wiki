#!/bin/bash

# site name
site="my-blog"

# 创建名为 <my-blog> 的站点，对应于本地一个文件夹
hugo new site $site && cd $site
# 初始化git仓库
git init
# 添加主题
git submodule add \
  git@github.com:hugo-sid/hugo-blog-awesome.git \
  themes/hugo-blog-awesome
# 写入配置文件
echo "theme = 'hugo-blog-awesome'" >> config.toml
# 复制示例站点中的博客
cp themes/hugo-blog-awesome/exampleSite/content/* -r content
# 这篇文章格式有点问题，先移除
rm -rf content/posts/rich-content
# 启用本地服务器
hugo server

