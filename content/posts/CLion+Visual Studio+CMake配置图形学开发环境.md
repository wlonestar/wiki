---
title: "CLion+Visual Studio+CMake配置图形学开发环境"
date: 2022-04-10T11:06:53+08:00
categories:
  - 过程记录
tags:
  - CLion
  - Visual Studio
  - OpenGL
math: true
hidden: false
draft: false
toc: true
---



这学期的计算机图形学需要使用 OpenGL ，我参照 [LearnOpenGL CN](https://learnopengl-cn.github.io/01%20Getting%20started/02%20Creating%20a%20window/) 配置好了环境，但是在使用 Visual Studio 写代码的过程中，发现很不顺手（是JetBrains的CLion太好用了），就想着用 CLion 再次配置一下。

因为 CLion 的构建工具是 CMake ，我不是很懂这个，就去查了很多资料，忙活了一下午才弄好，写下来怕忘了。

**请先按照 Learn OpenGL 的官方教程用 Visual Studio 配置，下面所有步骤建立在 VS 环境配置好的基础上**

## 创建新项目

创建 C++ Executable 项目，命名为 `opengl-project` ，刚创建完的 `CMakelist.txt` 应该是这样的

```cmake
cmake_minimum_required(VERSION 3.21)
project(opengl_project)

set(CMAKE_CXX_STANDARD 11)

add_executable(opengl_project main.cpp)
```

## 修改编译工具为 VS

在 `Settings -> Build, Executable, Deployment -> Toolchains` 中添加 Visual Studio ，

`Toolset` 选择 VS 安装路径， `Architecture` 选择 `amd64` ，其他保持默认即可

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2022-04-10_14-03-27.png)

## 添加 `include` 和 `lib` 目录

将头文件和库文件添加到项目目录下

```shell
├─cmake-build-debug
├─include
│  ├─glad
│  ├─GLFW
│  ├─glm
│  ├─KHR
│  └─stb_image.h
├─lib
│  └─glfw3.lib
├─CMakeLists.txt
├─glad.c
└─main.cpp
```

在 `CMakeLists.txt` 中添加这两个目录

```cmake
include_directories(include)
link_directories(lib)
# link option
set(option
        glfw3
        opengl32
)
```

## 示例程序

### hello triangle 程序

新建文件 `hello_triangle.cpp`

```cpp
// hello_triangle.cpp
//
// Created by wjl15 on 2022/4/9.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initilize GLAD\n";
    return -1;
  }

  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertex_shader);
  int success;
  char info_log[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << "\n";
  }
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << "\n";
  }

  unsigned int shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << "\n";
  }
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  float vertices[] = {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f, 0.5f, 0.0f
  };

  unsigned int vbo, vao;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteProgram(shader_program);

  glfwTerminate();
  return 0;
}
```

在 `CMakeLists.txt` 中添加构建目标

```cmake
add_executable(hello_triangle hello_triangle.cpp glad.c)
target_link_libraries(hello_triangle ${option})
```

运行结果

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2022-04-10_14-29-27.png)

### shader class 程序

新建文件 `shader_class.cpp` ，在这个程序里我们调用外部着色器文件

#### 添加 GLSL 插件

写着色器就有代码高亮了

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2022-04-10_14-19-47.png)

#### 编写 `shader` 类

创建 `shader.h` 头文件

```h
// shader.h
//
// Created by wjl15 on 2022/4/9.
//

#ifndef OPENGL_PROJECT_SHADER_H
#define OPENGL_PROJECT_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class shader {
private:
  static void check_compile_errors(GLuint shader, const std::string &type);

public:
  unsigned int id;

  shader(const char *vertex_path, const char *fragment_path, const char *geometry_path = nullptr);

  void use() const;
  void set_bool(const std::string &name, bool value) const;
  void set_int(const std::string &name, int value) const;
  void set_float(const std::string &name, float value) const;

  void set_vec2(const std::string &name, const glm::vec2 &value) const;
  void set_vec2(const std::string &name, float x, float y) const;

  void set_vec3(const std::string &name, const glm::vec3 &value) const;
  void set_vec3(const std::string &name, float x, float y, float z) const;

  void set_vec4(const std::string &name, const glm::vec4 &value) const;
  void set_vec4(const std::string &name, float x, float y, float z, float w) const;

  void set_mat2(const std::string &name, const glm::mat2 &mat) const;
  void set_mat3(const std::string &name, const glm::mat3 &mat) const;
  void set_mat4(const std::string &name, const glm::mat4 &mat) const;
};

void shader::check_compile_errors(const GLuint shader, const std::string &type) {
  GLint success;
  GLchar info_log[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, nullptr, info_log);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log << "\n";
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, nullptr, info_log);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log << "\n";
    }
  }
}

shader::shader(const char *vertex_path, const char *fragment_path, const char *geometry_path) {
  std::string vertex_code;
  std::string fragment_code;
  std::string geometry_code;
  std::ifstream v_shader_file;
  std::ifstream f_shader_file;
  std::ifstream g_shader_file;

  v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  g_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    v_shader_file.open(vertex_path);
    f_shader_file.open(fragment_path);
    std::stringstream v_shader_stream, f_shader_stream;
    v_shader_stream << v_shader_file.rdbuf();
    f_shader_stream << f_shader_file.rdbuf();
    v_shader_file.close();
    f_shader_file.close();
    vertex_code = v_shader_stream.str();
    fragment_code = f_shader_stream.str();
    if (geometry_path != nullptr) {
      g_shader_file.open(geometry_path);
      std::stringstream g_shader_stream;
      g_shader_stream << g_shader_file.rdbuf();
      g_shader_file.close();
      geometry_code = g_shader_stream.str();
    }
  } catch (std::ifstream::failure &e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << "\n";
  }

  const char *v_shader_code = vertex_code.c_str();
  const char *f_shader_code = fragment_code.c_str();
  unsigned int vertex, fragment;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &v_shader_code, nullptr);
  glCompileShader(vertex);
  check_compile_errors(vertex, "VERTEX");
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &f_shader_code, nullptr);
  glCompileShader(fragment);
  check_compile_errors(fragment, "FRAGMENT");

  unsigned int geometry = 0;
  if (geometry_path != nullptr) {
    const char *g_shader_code = geometry_code.c_str();
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &g_shader_code, nullptr);
    glCompileShader(geometry);
    check_compile_errors(geometry, "GEOMETRY");
  }

  id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  if (geometry_path != nullptr) {
    glAttachShader(id, geometry);
  }
  glLinkProgram(id);
  check_compile_errors(id, "PROGRAM");

  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (geometry_path != nullptr) {
    glDeleteShader(geometry);
  }
}

void shader::use() const {
  glUseProgram(id);
}

void shader::set_bool(const std::string &name, const bool value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

void shader::set_int(const std::string &name, const int value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void shader::set_float(const std::string &name, const float value) const {
  glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void shader::set_vec2(const std::string &name, const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void shader::set_vec2(const std::string &name, const float x, const float y) const {
  glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void shader::set_vec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void shader::set_vec3(const std::string &name, const float x, const float y, const float z) const {
  glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void shader::set_vec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void shader::set_vec4(const std::string &name, const float x, const float y, const float z, const float w) const {
  glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void shader::set_mat2(const std::string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void shader::set_mat3(const std::string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void shader::set_mat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

#endif//OPENGL_PROJECT_SHADER_H

```

#### 添加 `shader` 目录

统一存放着色器文件

```shell
├─cmake-build-debug
├─include
│  ├─glad
│  ├─GLFW
│  ├─glm
│  ├─KHR
│  └─stb_image.h
├─lib
│  └─glfw3.lib
├─shader
├─CMakeLists.txt
├─glad.c
└─main.cpp
```

在 `shader` 路径下新建 `3-3.vert` 和 `3-3.frag` 着色器文件

```glsl
// 3-3.vert
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
///////////////////////////////////////
// 3-3.frag
#version 330 core

out vec4 FragColor;

in vec3 ourColor;

void main() {
    FragColor = vec4(ourColor, 1.0f);
}
```

#### 程序代码

```cpp
// shader_class.cpp
//
// Created by wjl15 on 2022/4/9.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initilize GLAD\n";
    return -1;
  }

  shader our_shader("shader/3-3.vert", "shader/3-3.frag");

  float vertices[] = {
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  unsigned int vbo, vao;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    our_shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

  glfwTerminate();
  return 0;
}
```

在 `CMakeLists.txt` 中添加构建目标

```cmake
add_executable(shader_class shader_class.cpp glad.c)
target_link_libraries(shader_class ${option})
file(COPY
        ${CMAKE_CURRENT_SOURCE_DIR}/shader
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
```

运行结果

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2022-04-10_14-29-45.png)

### textures combined 程序

新建文件 `textures_combined.cpp` ，在这个程序里我们加载和创建纹理

#### 添加 `image` 目录

统一存放 图片文件

```shell
├─cmake-build-debug
├─include
│  ├─glad
│  ├─GLFW
│  ├─glm
│  ├─KHR
│  └─stb_image.h
├─lib
│  └─glfw3.lib
├─shader
├─image
├─CMakeLists.txt
├─glad.c
└─main.cpp
```

添加图片 `container.jpg` 和 `awesomeface.png`

#### 编写着色器文件

创建 `4-2.vert` 和 `4-2.frag` 文件

```glsl
// 4-2.vert
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
///////////////////////////////////////
// 4-2.frag
#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
```

#### 程序代码

```cpp
// textures_combined.cpp
//
// Created by wjl15 on 2022/4/9.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initilize GLAD\n";
    return -1;
  }

  shader our_shader("shader/4-2.vert", "shader/4-2.frag");

  float vertices[] = {
      0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
  };

  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3
  };

  unsigned int vbo, vao, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int texture1, texture2;

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;

  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("image/container.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture\n";
  }
  stbi_image_free(data);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("image/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture\n";
  }
  stbi_image_free(data);

  our_shader.use();
  glUniform1i(glGetUniformLocation(our_shader.id, "texture1"), 0);
  our_shader.set_int("texture2", 1);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    our_shader.use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);

  glfwTerminate();
  return 0;
}
```

在 `CMakeLists.txt` 中添加构建目标

```cmake
add_executable(textures_combined textures_combined.cpp glad.c)
target_link_libraries(textures_combined ${option})
file(COPY
        ${CMAKE_CURRENT_SOURCE_DIR}/shader
        ${CMAKE_CURRENT_SOURCE_DIR}/image
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
```

运行结果

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2022-04-10_14-37-14.png)

### cameara class 程序

新建文件 `camera_class.cpp` ，在这个程序里我们调用摄像机类来浏览场景

#### 编写 `camera` 类

创建 `camera.h` 头文件

```h
// camera.h
//
// Created by wjl15 on 2022/4/9.
//

#ifndef OPENGL_PROJECT_CAMERA_H
#define OPENGL_PROJECT_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum camera_movement {
  FORWARD, BACKWARD, LEFT, RIGHT
};

const float YAM = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class camera {
private:
  void update_camera_vectors();

public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 world_up;

  float yaw;
  float pitch;
  float movement_speed;
  float mouse_sensitivity;
  float zoom;

  explicit camera(glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f),
                  float _yam = YAM, float _pitch = PITCH)
      : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED),
        mouse_sensitivity(SENSITIVITY), zoom(ZOOM) {
    position = _pos;
    world_up = _up;
    yaw = _yam;
    pitch = _pitch;
    update_camera_vectors();
  }

  camera(float pos_x, float pos_y, float pos_z,
         float up_x, float up_y, float up_z, float _yaw, float _pitch)
      : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED),
        mouse_sensitivity(SENSITIVITY), zoom(ZOOM) {
    position = glm::vec3(pos_x, pos_y, pos_z);
    world_up = glm::vec3(up_x, up_y, up_z);
    yaw = _yaw;
    pitch = _pitch;
    update_camera_vectors();
  }

  glm::mat4 get_view_matrix() const;

  void process_keyboard(camera_movement direction, float delta_time);
  void process_mouse_movement(float x_offset, float y_offset, GLboolean constrain_pitch = true);
  void process_mouse_scroll(float y_offset);
};

void camera::update_camera_vectors() {
  glm::vec3 _front;
  _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  _front.y = sin(glm::radians(pitch));
  _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(_front);
  right = glm::normalize(glm::cross(front, world_up));
  up = glm::normalize(glm::cross(right, front));
}

glm::mat4 camera::get_view_matrix() const {
  return glm::lookAt(position, position + front, up);
}

void camera::process_keyboard(camera_movement direction, float delta_time) {
  float velocity = movement_speed * delta_time;
  if (direction == FORWARD) {
    position += front * velocity;
  }
  if (direction == BACKWARD) {
    position -= front * velocity;
  }
  if (direction == LEFT) {
    position -= right * velocity;
  }
  if (direction == RIGHT) {
    position += right * velocity;
  }
}

void camera::process_mouse_movement(float x_offset, float y_offset, GLboolean constrain_pitch) {
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;
    yaw += x_offset;
    pitch += y_offset;
    if (constrain_pitch) {
      if (pitch > 89.0f) {
        pitch = 89.0f;
      }
      if (pitch < -89.0f) {
        pitch = -89.0f;
      }
    }
    update_camera_vectors();
}

void camera::process_mouse_scroll(float y_offset) {
  zoom -= (float) y_offset;
  if (zoom < 1.0f) {
    zoom = 1.0f;
  }
  if (zoom > 1.0f) {
    zoom = 45.0f;
  }
}

#endif//OPENGL_PROJECT_CAMERA_H
```

#### 编写着色器文件

创建 `7-1.vert` 和 `7-1.frag` 文件

```glsl
// 7-1.vert
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
///////////////////////////////////////
// 7-1.frag
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
```

#### 程序代码

```cpp
// camera_class.cpp
//
// Created by wjl15 on 2022/4/9.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double x_pos_in, double y_pos_in);
void scroll_callback(GLFWwindow *window, double x_offset, double y_offset);
void process_input(GLFWwindow *window);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float last_x = WIDTH / 2.0f;
float last_y = HEIGHT / 2.0f;
bool first_mouse = true;

float delta_time = 0.0f;
float last_frame = 0.0f;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initilize GLAD\n";
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  shader our_shader("shader/7-1.vert", "shader/7-1.frag");

  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f,  2.0f, -2.5f),
      glm::vec3(1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
  };

  unsigned int vbo, vao;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int texture1, texture2;

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;

  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("image/container.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture\n";
  }
  stbi_image_free(data);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("image/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture\n";
  }
  stbi_image_free(data);

  our_shader.use();
  our_shader.set_int("texture1", 0);
  our_shader.set_int("texture2", 1);

  while (!glfwWindowShouldClose(window)) {
    auto current_frame = static_cast<float>(glfwGetTime());
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    our_shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
    our_shader.set_mat4("projection", projection);

    glm::mat4 view = camera.get_view_matrix();
    our_shader.set_mat4("view", view);

    glBindVertexArray(vao);

    for (unsigned int i= 0; i < 5; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      our_shader.set_mat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double x_pos_in, double y_pos_in) {
  float x_pos = static_cast<float>(x_pos_in);
  float y_pos = static_cast<float>(y_pos_in);

  if (first_mouse) {
    last_x = x_pos;
    last_y = y_pos;
    first_mouse = false;
  }

  float x_offset = x_pos - last_x;
  float y_offset = last_y - y_pos;

  last_x = x_pos;
  last_y = y_pos;

  camera.process_mouse_movement(x_offset, y_offset);
}

void scroll_callback(GLFWwindow *window, double x_offset, double y_offset) {
  camera.process_mouse_scroll(static_cast<float>(y_offset));
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.process_keyboard(FORWARD, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.process_keyboard(BACKWARD, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.process_keyboard(LEFT, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.process_keyboard(RIGHT, delta_time);
  }
}
```

在 `CMakeLists.txt` 中添加构建目标

```cmake
add_executable(camera_class camera_class.cpp glad.c)
target_link_libraries(camera_class ${option})
file(COPY
        ${CMAKE_CURRENT_SOURCE_DIR}/shader
        ${CMAKE_CURRENT_SOURCE_DIR}/image
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
```

运行结果

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_2022-04-10_14-48-36.png)

至此差不多基础配置都已完成，应该能正常写代码了

--完--

