---
title: "用C语言实现面向对象风格的泛型容器库"
date: 2023-07-02T20:15:05+08:00
categories:
  - 知识整理
tags:
  - C
  - 宏
  - 数据结构
math: true
hidden: false
draft: false
toc: true
---

<!-- cover -->

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/02c03aea0cfd4a398eb8760bc391141f.jpg)

---

<!-- main content -->

C 语言是不包含面向对象特性的，但是我们可以使用一些 tricks 来实现面向对象，借助 C 语言的宏，我们甚至可以实现泛型。

本文以双向循环链表为例，讲解如何实现面向对象风格的泛型链表。

## 传统实现

一般情况下我们会这样定义链表：

```c
typedef struct list_node {
  struct list_node *prev, *next;
  int data;
} list_node;
typedef struct list {
  list_node *_head;
  size_t _size;
} list;
static list *init_list() {
  list *obj = (list *) malloc(sizeof(list));
  obj->_head = (list_node *) malloc(sizeof(list_node));
  obj->_head->prev = obj->_head;
  obj->_head->next = obj->_head;
  obj->_size = 0;
  return obj;
}
static void free_list(list *self) {
  while (self->_size > 0) {
    list_pop_back(self);
  }
  free(self->_head);
  free(self);
}
```

然后实现插入、删除算法等

```c
static void list_push_back(list *self, int value) {
  list_node *new_node = (list_node *) malloc(sizeof(list_node));
  new_node->data = value;
  new_node->prev = self->_head->prev;
  new_node->next = self->_head;
  self->_head->prev->next = new_node;
  self->_head->prev = new_node;
  self->_size++;
}

static void list_pop_back(list *self) {
  list_node *removal = self->_head->prev;
  removal->prev->next = self->_head;
  self->_head->prev = removal->prev;
  free(removal);
  self->_size--;
}
```

测试一下代码：

```c
int main() {
  list *l = init_list();
  for (int i = 0; i < 5; i++) {
    list_push_back(l, i);
  }
  assert(l->_size == 5);
  for (list_node *p = l->_head->next; p != l->_head; p = p->next) {
    printf("%d ", p->data);
  }
  printf("\n");
  for (int i = 0; i < 5; i++) {
    list_pop_back(l);
  }
  assert(l->_size == 0);
  free_list(l);
}
```

可以看到期望的输出：

```
0 1 2 3 4
```

基于上面的代码，将其改造成面向对象风格。

## 面向对象

C语言的结构体`struct`是不支持成员方法的，但是我们可以使用函数指针实现。

比如上面的链表，将 `push_back` 和 `pop_back` 写成函数指针的形式为：

```c
typedef struct list {
  list_node *_head;
  size_t _size;
  void (*push_back)(struct list *self, int value);
  void (*pop_back)(struct list *self);
} list;
```

初始化链表应改写为：

```c
static list *init_list() {
  list *obj = (list *) malloc(sizeof(list));
  obj->_head = (list_node *) malloc(sizeof(list_node));
  obj->_head->prev = obj->_head;
  obj->_head->next = obj->_head;
  obj->_size = 0;
  obj->push_back = list_push_back;
  obj->pop_back = list_pop_back;
  return obj;
}
```

这样的话，我们就可以像C++那样调用成员方法了：

```c
int main() {
  list *l = init_list();
  for (int i = 0; i < 5; i++) {
    l->push_back(l, i);
  }
  assert(l->_size == 5);
  for (list_node *p = l->_head->next; p != l->_head; p = p->next) {
    printf("%d ", p->data);
  }
  printf("\n");
  for (int i = 0; i < 5; i++) {
    l->pop_back(l);
  }
  assert(l->_size == 0);
  free_list(l);
}
```

## 泛型

C语言实现泛型最简单的方法个人认为是宏。虽然可读性很差，并且不方便调试，但是实现起来简单。

我们对上面的代码进行改写，将函数都写进宏内部：

```c
#define DECLARE_LIST(T)                                                        \
  typedef struct list_node {                                                   \
    struct list_node *prev, *next;                                             \
    T data;                                                                    \
  } list_node;                                                                 \
  typedef struct list {                                                        \
    list_node *_head;                                                          \
    size_t _size;                                                              \
    void (*push_back)(struct list * self, T value);                            \
    void (*pop_back)(struct list * self);                                      \
  } list;                                                                      \
  static void list_push_back(list *self, int value) {                          \
    list_node *new_node = (list_node *)malloc(sizeof(list_node));              \
    new_node->data = value;                                                    \
    new_node->prev = self->_head->prev;                                        \
    new_node->next = self->_head;                                              \
    self->_head->prev->next = new_node;                                        \
    self->_head->prev = new_node;                                              \
    self->_size++;                                                             \
  }                                                                            \
  static void list_pop_back(list *self) {                                      \
    list_node *removal = self->_head->prev;                                    \
    removal->prev->next = self->_head;                                         \
    self->_head->prev = removal->prev;                                         \
    free(removal);                                                             \
    self->_size--;                                                             \
  }                                                                            \
  static list *init_list() {                                                   \
    list *obj = (list *)malloc(sizeof(list));                                  \
    obj->_head = (list_node *)malloc(sizeof(list_node));                       \
    obj->_head->prev = obj->_head;                                             \
    obj->_head->next = obj->_head;                                             \
    obj->_size = 0;                                                            \
    obj->push_back = list_push_back;                                           \
    obj->pop_back = list_pop_back;                                             \
    return obj;                                                                \
  }                                                                            \
  static void free_list(list *self) {                                          \
    while (self->_size > 0) {                                                  \
      self->pop_back(self);                                                     \
    }                                                                          \
    free(self->_head);                                                         \
    free(self);                                                                \
  }

#define CREATE_LIST(obj) list *obj = init_list();

#define FREE_LIST(obj) free_list(obj);

DECLARE_LIST(int)

int main() {
  CREATE_LIST(l);
  for (int i = 0; i < 5; i++) {
    l->push_back(l, i);
  }
  assert(l->_size == 5);
  for (list_node *p = l->_head->next; p != l->_head; p = p->next) {
    printf("%d ", p->data);
  }
  printf("\n");
  for (int i = 0; i < 5; i++) {
    l->pop_back(l);
  }
  assert(l->_size == 0);
  FREE_LIST(l);
}
```

代码由三个宏构成，`DECLARE_LIST` 定义链表，`CREATE_LIST` 创建链表对象，`FREE_LIST` 销毁对象。

但是这个代码有致命的缺陷：它只能定义一个类型的链表。

如果使用两次`DECLARE_LIST` 会出现符号重定义的错误，我们还需要对它进行改造。

这里使用了一点宏的拼接技巧，解决了符号重定义的问题。

```c
#define DECLARE_LIST(list, T)                                                  \
  typedef struct list##_node {                                                 \
    struct list##_node *prev, *next;                                           \
    T data;                                                                    \
  } list##_node;                                                               \
  typedef struct list {                                                        \
    list##_node *_head;                                                        \
    size_t _size;                                                              \
    void (*push_back)(struct list * self, int value);                          \
    void (*pop_back)(struct list * self);                                      \
  } list;                                                                      \
  static void _##list##_push_back(list *self, int value) {                     \
    list##_node *new_node = (list##_node *)malloc(sizeof(list##_node));        \
    new_node->data = value;                                                    \
    new_node->prev = self->_head->prev;                                        \
    new_node->next = self->_head;                                              \
    self->_head->prev->next = new_node;                                        \
    self->_head->prev = new_node;                                              \
    self->_size++;                                                             \
  }                                                                            \
  static void _##list##_pop_back(list *self) {                                 \
    list##_node *removal = self->_head->prev;                                  \
    removal->prev->next = self->_head;                                         \
    self->_head->prev = removal->prev;                                         \
    free(removal);                                                             \
    self->_size--;                                                             \
  }                                                                            \
  static list *_##list##_init_list() {                                         \
    list *obj = (list *)malloc(sizeof(list));                                  \
    obj->_head = (list##_node *)malloc(sizeof(list##_node));                   \
    obj->_head->prev = obj->_head;                                             \
    obj->_head->next = obj->_head;                                             \
    obj->_size = 0;                                                            \
    obj->push_back = _##list##_push_back;                                      \
    obj->pop_back = _##list##_pop_back;                                        \
    return obj;                                                                \
  }                                                                            \
  static void _##list##_free_list(list *self) {                                \
    while (self->_size > 0) {                                                  \
      self->pop_back(self);                                                    \
    }                                                                          \
    free(self->_head);                                                         \
    free(self);                                                                \
  }

#define CREATE_LIST(list, obj) list *obj = _##list##_init_list();

#define FREE_LIST(list, obj) _##list##_free_list(obj);

DECLARE_LIST(flist, float)

int main() {
  CREATE_LIST(flist, l);
  for (int i = 0; i < 5; i++) {
    l->push_back(l, i);
  }
  assert(l->_size == 5);
  for (flist_node *p = l->_head->next; p != l->_head; p = p->next) {
    printf("%f ", p->data);
  }
  printf("\n");
  for (int i = 0; i < 5; i++) {
    l->pop_back(l);
  }
  assert(l->_size == 0);
  FREE_LIST(flist, l);
}
```

现在我们就有了一个面向对象风格的泛型链表了。

## 更进一步

不妨趁着学数据结构的时候试着将常用的数据结构也用C语言实现一遍，
这样在库函数少之又少的C语言里，我们也可以优雅的使用高级的数据结构了。

我的实现：[wlonestar/libcoo](https://github.com/wlonestar/libcoo)

因为学业，没有足够的时间，目前只实现了一部分容器，供大家参考。

<!-- end -->

---

以上。
