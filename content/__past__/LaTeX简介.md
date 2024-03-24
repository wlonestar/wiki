---
title: "LaTeX简介"
date: 2021-03-27T10:19:53+08:00
draft: false
toc: true
description: 一篇极简的 LaTeX 介绍文章
image: https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/2021-03-27.jpg
---



我们平时在word里新建一个doc(x)文件后，输入内容，调整页边距、字体大小、行间距等待，然后交给打印机打印就可以了。

如果你写了一篇关于新疆棉花的文章，2000字，在word里调整很快就可以得到你想要的效果。

但如果你想把自己记下的关于矩阵乘法的笔记打印下来，因为你的字太丑了，你就会发现遇到了困难：那些数学符号应该怎么排版才好看，尤其是矩阵这样的大块内容。

![image-20210331154208560](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210331154208560.png)

于是你通过找资料知道了怎么在word里插入公式，但每次都要点击插入-公式（或 Alt + =），才能在当前位置插入公式块。

![image-20210327134551400](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210327134551400.png)

想要加一个下标还要点击上下标-下标。

![image-20210327134643470](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210327134643470.png)

可能你注意到了word有一个墨迹公式的功能，于是你迫不及待地打开，发现，这玩意儿也是个垃圾，手写不适合鼠标，或许对触摸屏好一点。

![image-20210327135013736](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210327135013736.png)

![image-20210331154254678](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210331154254678.png)

最终你通过万能的互联网，发现了LaTeX这个好东西。

![image-20210331154310557](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210331154310557.png)

# LaTeX诞生背景

**高德纳**，图中的老爷子出生于美国，是著名计算机科学家，斯坦福大学计算机系荣誉退休教授。高德纳教授为现代计算机科学的先驱人物，创造了算法分析的领域，在数个理论计算机科学的分支做出基石一般的贡献。在计算机科学及数学领域发表了多部具广泛影响的论文和著作。1974年图灵奖得主。

1966年，高德纳的著作达到3000多页，它与出版社商定，编纂一部计算机程序设计巨著《计算机程序设计艺术》，到2018年这本书已经出版了4卷，对计算机领域产生了深远影响。

![KnuthAtOpenContentAlliance](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/KnuthAtOpenContentAlliance.jpg)

但在当时，电脑排版技术十分粗糙，已经影响到他的著作的印刷质量。他以典型的黑客思维模式，决定自行编写一个排版软件：TeX。他计划在1978年的学术休假时完成这项工作，但事实上，直到十多年之后的1989年，TeX语言才可以使用。

# TeX和LaTeX

![image-20210331154347835](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210331154347835.png)

**TeX**（音译“泰赫”，“泰克”，写作“TeX”），是一个由美国计算机教授高德纳（Donald Ervin Knuth）编写的排版软件。它在学术界特别是数学、物理学和计算机科学界十分流行。TeX被普遍认为是一个优秀的排版工具，尤其是对于复杂数学公式的处理。利用LaTeX等终端软件，TeX就能够排版出精美的文本以帮助人们辨认和查找。

![image-20210331154356881](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210331154356881.png)

**LaTeX**（拉泰克，写作“LATEX”），是一种基于TEX的排版系统，由美国计算机科学家莱斯利·兰伯特在20世纪80年代初期开发，利用这种格式系统的处理，即使用户没有排版和程序设计的知识也可以充分发挥由TEX所提供的强大功能，不必一一亲自去设计或校对，能在几天，甚至几小时内生成很多具有书籍质量的印刷品。对于生成复杂表格和数学公式，这一点表现得尤为突出。

# LaTeX主要功能

TeX本质上是一门宏语言。而LaTeX是基于这门宏语言，经过后人不断的完善形成的一种排版格式。

LaTex文件扩展名为`.tex`。创建文件后，输入代码，保存文件，通过编辑器提供的工具编译为LaTeX，如果运行正常则可看到导出的PDF文件。

![image-20210331154417707](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210331154417707.png)

## 文档结构

以article（文章）模板为例：

```latex
\documentclass[UTF8]{ctexart} %模板类型为中文article
\title{title} %标题
\author{author} %作者
\date{} %时间
\begin{document} %开始正文
\maketitle
\tableofcontents

\section{section1}
\subsection{subsection1}
这是文章第一节第一小节
\subsection{subsection2}
这是文章第一节第二小节

\section{section2}
\subsection{subsection1}
这是文章第二节第一小节
\subsection{subsection2}
这是文章第二节第二小节

\end{document}
```

![image-20210327145536868](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210327145536868.png)

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210331154443723.png)

## 数学公式

latex编辑公式相对于word而言，可以调整公式字母的大小选择合适的行距，使排版看上去更加整洁。而且虽然是输代码，但效率却是远高于鼠标点击的。

latex公式有两种，分别为行内公式和行间公式（行间公式有两种插入方式）：

```latex
一元二次方程 $ax^2+bx+c=0 (b^2-4ac \geqslant 0) $ 的解为：
\[x=\dfrac{-b\pm \sqrt{b^2-4ac}}{2a}\] 
```

输出结果：

![image-20210327150500862](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210327150500862.png)

![](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210331154443723.png)

## 公式块

输入数学公式一般都需要两个宏包：mathtools和amsmath。

通过查找官方文档或者借助编辑器插件，只需要鼠标点击就可以得到符号对应的代码。

```latex
定义：设 $A$ 是一个 $m \times n$ 矩阵，$B$ 是一个 $n \times s$ 矩阵，即
\[
A = \begin{pmatrix}
    a_{11} & a_{12} & \dots & a_{1n} \\
    a_{21} & a_{22} & \dots & a_{2n} \\
    \vdots & \vdots & \ddots & \vdots \\
    a_{m1} & a_{m2} & \dots & a_{mn} \\
\end{pmatrix}
,
B = \begin{pmatrix}
    b_{11} & b_{12} & \dots & b_{1s} \\
    b_{21} & b_{22} & \dots & b_{2s} \\
    \vdots & \vdots & \ddots & \vdots \\
    b_{n1} & b_{n2} & \dots & b_{ns} \\
\end{pmatrix}
.
\]
则 $A$ 与 $B$ 之乘积 $AB$ （记作 $C=(c_{ij})$ ）是一个 $m \times n$ 矩阵，且
\[
    c_{ij} = a_{i1}b_{1j} + a_{i2}b_{2j} + \dots + a_{im}b_{nj} = \sum_{k = 1}^{n} a_{ik}b_{kj}
\]
即矩阵 $C=AB$ 的第 $i$ 行第 $j$ 列元素 $c_{ij}$ ，是 $A$ 的第 $i$ 行 $n$ 个元素与 $B$ 的第 $j$ 列相应的 $n$ 个元素分别相乘的乘积之和。
```

![image-20210327162421218](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210327162421218.png)

![image-20210331154521069](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210331154521069.png)

可以对比一下，第一个是word插入公式，第二个是LaTeX生成的。

![image-20210331154535316](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210331154535316.png)

![image-20210331154542310](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210331154542310.png)

看到这里，应该感受到了LaTeX的强大之处了吧？

word入门及其简单，但学习更高级的功能很难；LaTeX入门阶段的学习曲线就及其陡峭，可一旦熟练就很得心应手，能排版出自己想要的格式。

![image-20210331203203852](https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/image-20210331203203852.png)

日常还是markdown用得多一点，比较公式的需求不大，而且便于互联网传播。

本篇文章只是做了一个很简单的介绍，想要了解更多内容，请自行查找资料。

最后还是要来一句：LaTeX牛逼！！！



