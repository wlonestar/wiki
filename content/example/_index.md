+++
outputs = ["Reveal"]
+++

# SpringBoot多模块项目搭建、运行、打包

最近在学 Spring Cloud，发现 maven 的多模块构建还不会，就先去查了以下资料，记录如下。

---

## 新建 maven 父工程

使用 idea 的 Spring Initializr 创建 Spring Boot 工程，只留下 `pom.xml` 文件，打包方式改为 `pom` 

```xml
<packaging>pom</packaging>
```

---

{{% section %}}

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

![](https://mypic-1305118058.cos.ap-hongkong.myqcloud.com/img/Snipaste_2022-01-04_10-11-56.png)

---

其中 `base` 、 `repository` 、 `service` 把 Spring Boot 的启动类和配置文件全部删除，把 `pom.xml` 文件中的 `build` 部分删除，父模块改成创建的父模块，下面是 `base` 子模块的 `pom.xml` 文件，其他两个同理

```xml
<project ...
  <parent>
    <groupId>com.wjl</groupId>
    <artifactId>spring-boot-multi</artifactId>
    <version>1.0.0</version>
  </parent>
  <artifactId>base</artifactId>
  <version>1.0.0</version>
  <name>base</name>
  <description>base</description>
  <dependencies>
    ...
  </dependencies>
</project>
```

---

注意 `web` 模块的 maven 依赖，要把这里的启动类移动到 `com.wjl` 包中，不然其他模块不能自动注入

```bash
└──src
  └──main
    └──java
      └──com
        └──wjl
          ├──WebApplication.java
          └──web
```

---

maven 打包插件中要标明程序入口类 `com.wjl.WebApplication` 

```xml
...
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
...
```

{{% /section %}}

---

## 添加依赖管理

我们要在父工程的 `pom.xml` 文件中添加依赖管理，声明子模块，并把打包插件改成 `maven-compiler-plugin`

```xml
...
  <modules>
    <module>base</module>
    <module>repository</module>
    ...
  </modules>
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
      ...
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
...
```

---

{{% section %}}

可以再写个测试用例打包测试一下，这里我就不写了，说一下打包过程。

直接在父工程打包即可，这里使用 idea ，有些可能要先 `compile` 一下，我这个示例比较简单，直接点一下 `package` 就行了

![](https://mypic-1305118058.cos.ap-hongkong.myqcloud.com/img/Snipaste_2022-01-04_10-26-58.png)

---

可以在 `web` 模块下的 `target` 目录下看到打包好的 `jar` 包

![](https://mypic-1305118058.cos.ap-hongkong.myqcloud.com/img/Snipaste_2022-01-04_10-29-19.png)

---

命令行使用命令 `java -jar web-1.0.0.jar` 运行

![](https://mypic-1305118058.cos.ap-hongkong.myqcloud.com/img/Snipaste_2022-01-04_10-31-13.png)

打包成功，可以顺利运行！

{{% /section %}}

---

--完--
