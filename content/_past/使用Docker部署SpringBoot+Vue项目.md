---
title: "使用Docker部署SpringBoot+Vue项目"
date: 2021-09-29T13:58:48+08:00
description: 记录首次成功利用 Docker 部署项目的全过程
categories:
- 过程记录
tags:
- Docker
- Spring Boot
- Vue
image: https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/2021-09-29.jpg
draft: false
toc: true
---



之前写好了一个SpringBoot + Vue 的项目，但是还不知道怎么部署上线，于是这几天稍微学了一下 Docker ，还只是会用的阶段，就先把部署过程写下来，防止遗忘。



## 测试项目编写

因为是测试使用，就先在本地简单写个前后端分离的小项目：后端向前端返回所有用户信息，前端简单的将信息展示出来，MySQL负责存储用户信息。

很简单，项目代码： [wlonestar/docker-test](https://github.com/wlonestar/docker-test)

### 数据库

因为后端使用 JPA 框架，所有只需要先创建 lblog 数据库即可

### 后端项目

创建 Spring Boot 项目，引入相关依赖：

```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
<dependency>
	<groupId>mysql</groupId>
    <artifactId>mysql-connector-java</artifactId>
</dependency>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-jpa</artifactId>
</dependency>
<dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
</dependency>
```

项目配置文件：

```yml
server:
  port: 8080
spring:
  datasource:
    driver-class-name: com.mysql.cj.jdbc.Driver
    url: jdbc:mysql://106.14.24.103:3306/lblog?useSSL=false&serverTimezone=UTC
    username: root
    password: wjl1237107
    hikari:
      minimum-idle: 1
      maximum-pool-size: 20
      initialization-fail-timeout: 60000
  jpa:
    show-sql: false
    hibernate:
      ddl-auto: update
    properties:
      hibernate:
        format_sql: true
    database-platform: org.hibernate.dialect.MySQL8Dialect
```

项目必要配置，可以去代码仓库查看

1. Entity，很简单的几个字段

```java
@Getter
@Setter
@Entity
@Table(name = "user")
public class User {
    
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id")
    private Long id;

    @Column(name = "username")
    private String username;

    @Column(name = "pasword")
    private String password;

    @Column(name = "age")
    private Integer age;

    @Column(name = "gendar")
    private Boolean gendar;

    @Column(name = "address")
    private String address;

}
```

2. Repository，因为使用了 JPA ，单表 CRUD 可以直接继承 `JpaRepository`

```java
@Repository
public interface UserRepository extends JpaRepository<User, Long> {
}
```

3. Service，项目过于简单，`Service` 就没必要写接口再实现了，直接写在一个类里面

```java
@Service
public class UserService {

    @Autowired
    private UserRepository userRepository;

    public List<User> findAll() {
        return userRepository.findAll();
    }

}
```

4. Controller，RESTful API 风格，返回格式已经封装好（参照[后端接口返回结果统一封装 (wangjialei.xyz)](https://blog.wangjialei.xyz/p/后端接口返回结果统一封装/)）

```java
@RestController
@RequestMapping("/user")
public class UserController {

    @Autowired
    private UserService userService;

    @RequestMapping(method = RequestMethod.GET)
    public List<User> findAll() {
        return userService.findAll();
    }

}
```

测试运行成功后，使用 Maven 打包工具打包

![image-20210929142440377](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210929142440377.png)

可以在 `target` 目录下看到打好的 `jar` 包

![image-20210929142706746](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210929142706746.png)

### 前端项目

因为后端就一个接口，所以前端只需要将后端接口数据取到再渲染出来就可以了

使用 vue/cli 创建 Vue3 项目，按照图中配置即可

![image-20210929143040689](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210929143040689.png)

进入项目后删除不必要的文件，具体看仓库

然后安装 axios

```shell
npm i axios -S
```

封装一个 axios 请求

```js
// utils/request.js
import axios from 'axios'

const request = axios.create({
  baseURL: 'http://106.14.24.103:8080', // 这里的地址先填 localhost:8088，方便测试效果
  timeout: 5000
})

export default request
```

然后再封装查询所有用户信息的请求

```js
// api/user.js
import request from '../utils/request'

// 查询所有用户
export function getAllUser () {
  return request({
    url: '/user',
    method: 'get'
  })
}
```

直接在首页展示

```vue
// Home.vue
<template>
  <div>
    <div v-for="user in users" :key="user" style="margin: 20px 20px 20px 20px;">
      <p>{{ user.id }}</p>
      <p>{{ user.username }}</p>
      <p>{{ user.password }}</p>
      <p>{{ user.age }}</p>
      <p v-if="user.gendar === true">男</p>
      <p v-if="user.gendar === false">女</p>
      <p>{{ user.address }}</p>
    </div>
  </div>
</template>

<script>
import { getAllUser } from '../api/user'
export default {
  name: 'Home',
  data () {
    return {
      users: []
    }
  },
  created () {
    this.load()
  },
  methods: {
    load () {
      getAllUser().then(data => {
        this.users = data.data.data
      })
    }
  }
}
</script>
```

执行 `npm run serve` 访问指定的端口没问题的话，打包

```shell
npm run build
```

可以看到项目打包到 `dist` 目录下

![image-20210929144032895](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210929144032895.png)

## Docker 部署上线

这个项目包括三个部分，Spring Boot 项目，Vue 打包好的静态页面和 MySQL 数据库。

Spring Boot 项目由于内置了 Tomcat 容器，所以只需要提供项目对应的 Java 运行环境即可，Vue 打包的静态页面可以通过 Nginx 服务器部署，而 MySQL 数据库还需要一个单独的容器运行。

所以我们这个项目需要三个容器： Java 容器，Nginx 容器和 MySQL 容器。

先在阿里云服务器实例安全组中开放 80、3306、8080 这三个端口，便于测试，目标网站会暴露在 80 端口。

### MySQL 容器

首先配置 MySQL 容器，拉取最新的 MySQL 镜像，取个名字，挂载 `/home/web/data` 目录下的数据，把容器内的 3306 端口映射到服务器的 3306 端口，设置 MySQL 的 root 密码

```shell
docker run --name mysql01-web -p 3306:3306 -v /home/web/data:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=123456 -d mysql
```

使用 `docker ps` 命令，查看当前正在运行的容器：

![image-20210929145308922](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210929145308922.png)

可以看到正在运行的有三个容器，最后一个 `mysql01-web` 就是创建的这个 MySQL 容器，使用数据库连接工具连接一下该数据库，连接之前确定 3306 端口开放

![image-20210929145502174](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210929145502174.png)

我们在服务器的数据库里创建对应的表，然后随便插入几条数据方便后面测试

### Java 容器

这里我们要自己写 Dockerfile

```Do
FROM openjdk:17
COPY *.jar /app.jar
ENTRYPOINT ["java","-jar","/app.jar"]
```

然后我们把打包好的 jar 文件和 Dockerfile 上传到服务器 `/home/web/server` 目录下

![image-20210929145832880](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210929145832880.png)

使用 `docker build` 命令构建镜像

```shell
docker build -t wjl/web .
```

运行 Java 容器

```shell
docker run -p 8080:8080 --name server01-web -d wjl/web
```

使用 `docker ps` 查看是否正常运行，若正常，访问 `ip:8080/user` 应该可以看到之前插入的几条数据

![image-20210929150352830](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210929150352830.png)

### Nginx 容器

同样的，我们在前端项目中写一个 Dockerfile

```do
FROM nginx
COPY dist/ /usr/share/nginx/html/
COPY nginx/default.conf /etc/nginx/conf.d/default.conf
```

然后再创建一个 `nginx` 目录，在目录下创建 nginx 配置文件 `default.conf`

```conf
server {
    listen       80;
    server_name  localhost;

    location / {
        root   /usr/share/nginx/html;
        index  index.html index.htm;
    }
}
```

把 `/dist` 、 `/nginx` 和 `Dockerfile` 一起上传到服务器 `/home/web/app` 目录下

![image-20210929150821712](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210929150821712.png)

使用 `docker build` 命令构建镜像

```shell
docker build -t wjl/nginx .
```

运行创建的 nginx 容器

```shell
docker run -p 80:80 -d --name nginx01-web wjl/nginx
```

使用 `docker ps` 查看是否正常运行，若正常，直接访问服务器公网 ip 应该可以看到数据了

![image-20210929151119771](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210929151119771.png)

完成！





