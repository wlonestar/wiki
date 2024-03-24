---
title: "SpringBoot多模块项目搭建、运行、打包"
date: 2022-01-04T09:53:50+08:00
description: 
image: https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/IMG20211018154346.jpg
draft: false
toc: true
---



最近在学 Spring Cloud，发现 maven 的多模块构建还不会，就先去查了以下资料，记录如下。



## 新建 maven 父工程

使用 idea 的 Spring Initializr 创建 Spring Boot 工程，只留下 `pom.xml` 文件，打包方式改为 `pom` 

```xml
<packaging>pom</packaging>
```

## 创建子模块

在项目目录下依次新建 `base` 、 `repository` 、 `service` 、 `web` 子模块

```bash
├───.idea
├───base
├───repository
├───service
├───web
└───pom.xml
```

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2022-01-04_10-11-56.png)

其中 `base` 、 `repository` 、 `service` 把 Spring Boot 的启动类和配置文件全部删除，把 `pom.xml` 文件中的 `build` 部分删除，父模块改成创建的父模块，下面是 `base` 子模块的 `pom.xml` 文件，其他两个同理

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <parent>
        <groupId>com.wjl</groupId>
        <artifactId>spring-boot-multi</artifactId>
        <version>1.0.0</version>
    </parent>

    <artifactId>base</artifactId>
    <version>1.0.0</version>
    <name>base</name>
    <description>base</description>

    <properties>
        <java.version>17</java.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
        </dependency>
    </dependencies>

</project>
```

注意 `web` 模块的 maven 依赖，要把这里的启动类移动到 `com.wjl` 包中，不然其他模块不能自动注入

```bash
└───src
    └───main
        └───java
            └───com
                └───wjl
                    ├───WebApplication.java
                    └───web
```

maven 打包插件中要标明程序入口类 `com.wjl.WebApplication` 

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <parent>
        <groupId>com.wjl</groupId>
        <artifactId>spring-boot-multi</artifactId>
        <version>1.0.0</version>
    </parent>

    <artifactId>web</artifactId>
    <version>1.0.0</version>
    <name>web</name>
    <description>web</description>

    <packaging>jar</packaging>

    <properties>
        <java.version>17</java.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <dependency>
            <groupId>com.wjl</groupId>
            <artifactId>service</artifactId>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
                <configuration>
                    <mainClass>com.wjl.WebApplication</mainClass>
                </configuration>
            </plugin>
        </plugins>
    </build>

</project>
```

## 添加依赖管理

我们要在父工程的 `pom.xml` 文件中添加依赖管理，声明子模块，并把打包插件改成 `maven-compiler-plugin`

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.wjl</groupId>
    <artifactId>spring-boot-multi</artifactId>
    <version>1.0.0</version>
    <name>spring-boot-multi</name>
    <description>spring-boot-multi</description>

    <packaging>pom</packaging>

    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.6.2</version>
        <relativePath/>
    </parent>

    <modules>
        <module>base</module>
        <module>repository</module>
        <module>service</module>
        <module>web</module>
    </modules>

    <properties>
        <java.version>17</java.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>

    <dependencyManagement>
        <dependencies>
            <dependency>
                <groupId>com.wjl</groupId>
                <artifactId>base</artifactId>
                <version>1.0.0</version>
            </dependency>
            <dependency>
                <groupId>com.wjl</groupId>
                <artifactId>repository</artifactId>
                <version>1.0.0</version>
            </dependency>
            <dependency>
                <groupId>com.wjl</groupId>
                <artifactId>service</artifactId>
                <version>1.0.0</version>
            </dependency>
            <dependency>
                <groupId>com.wjl</groupId>
                <artifactId>web</artifactId>
                <version>1.0.0</version>
            </dependency>
        </dependencies>
    </dependencyManagement>

    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
            </plugin>
        </plugins>
    </build>

</project>
```

可以再写个测试用例打包测试一下，这里我就不写了，说一下打包过程。

直接在父工程打包即可，这里使用 idea ，有些可能要先 `compile` 一下，我这个示例比较简单，直接点一下 `package` 就行了

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2022-01-04_10-26-58.png)

可以在 `web` 模块下的 `target` 目录下看到打包好的 `jar` 包

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2022-01-04_10-29-19.png)

命令行使用命令 `java -jar web-1.0.0.jar` 运行

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2022-01-04_10-31-13.png)

打包成功，可以顺利运行！



--完--
