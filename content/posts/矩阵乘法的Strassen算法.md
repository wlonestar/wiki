---
title: 矩阵乘法的Strassen算法
date: 2022-08-31T19:06:45+08:00
description: 时间复杂度O(n^2.81) (rust 语言实现)
image: https://image-1305118058.cos.ap-nanjing.myqcloud.com/image/Snipaste_202-05-01_23-05-54.jpg
categories:
  - 学习笔记
tags:
  - rust
  - 算法
  - 矩阵
math: true
hidden: false
draft: false
toc: true
---

> 
> 矩阵乘法的定义：
> 
> 若 $A=(a_{ij})$ 和 $B=(b_{ij})$ 是 $n \times n$ 的方阵， 则对 $i, j = 1, 2, ..., n$，定义乘积 $C=A\cdot B$ 中的元素 $c_{ij}$ 为：
> 
> $$ c_{ij} = \sum_{k=1}^{n} a_{ik} \cdot b_{kj} $$

按照矩阵乘法的定义不难写出算法1

```rs
// 算法1：定义法
pub fn square_matrix_multiply<T: Default + Clone + AddAssign + Mul<Output = T>>(
    a: &Vec<Vec<T>>,
    b: &Vec<Vec<T>>,
) -> Vec<Vec<T>>
where
    T: Mul<T>,
{
    let mut c = vec![vec![T::default(); a.len()]; a.len()];
    if a.len() == a[0].len() && b.len() == a[0].len() && a.len() == b.len() {
        let n = a.len();
        for i in 0..n {
            for j in 0..n {
                c[i][j] = T::default();
                for k in 0..a.len() {
                    c[i][j] += a[i][k].clone() * b[k][j].clone();
                }
            }
        }
    }
    return c;
}
```

由于三重 `for` 循环的每一重都恰好执行 `n` 步，因此该算法的时间复杂度为 $O(n^3)$

为此我们尝试改进

## 一个简单的分治算法

为简单起见，假定三个矩阵均为 $n \times n$ 矩阵，$n$ 是 $2$ 的幂，矩阵 $A, B, C$ 均分解为 $4$ 个 $n / 2 \times n / 2$ 的子矩阵，则计算公式等价于

$$ C_{11} = A_{11} \cdot B_{11} + A_{12} \cdot B_{21} $$

$$ C_{12} = A_{11} \cdot B_{12} + A_{12} \cdot B_{22} $$

$$ C_{21} = A_{21} \cdot B_{11} + A_{22} \cdot B_{21} $$

$$ C_{22} = A_{21} \cdot B_{12} + A_{22} \cdot B_{22} $$

我们可以设计一个直接的递归分治算法2

```rs
// 算法2：直接分治法
pub fn square_matrix_multiply_recursive<
    T: Default + Clone + AddAssign + Mul<Output = T> + Add<Output = T> + Debug,
>(
    a: &Vec<Vec<T>>,
    b: &Vec<Vec<T>>,
) -> Vec<Vec<T>>
where
    T: Mul<T> + Add<T>,
{
    let mut n = a.len();
    let mut c = vec![vec![T::default(); n]; n];
    if n == 1 {
        c[0][0] = a[0][0].clone() * b[0][0].clone();
    } else {
        n /= 2;
        let mut a11 = vec![vec![T::default(); n]; n];
        let mut a12 = vec![vec![T::default(); n]; n];
        let mut a21 = vec![vec![T::default(); n]; n];
        let mut a22 = vec![vec![T::default(); n]; n];
        let mut b11 = vec![vec![T::default(); n]; n];
        let mut b12 = vec![vec![T::default(); n]; n];
        let mut b21 = vec![vec![T::default(); n]; n];
        let mut b22 = vec![vec![T::default(); n]; n];
        partition_four(&a, &mut a11, &mut a12, &mut a21, &mut a22);
        partition_four(&b, &mut b11, &mut b12, &mut b21, &mut b22);
        let c11 = square_matrix_add(
            &square_matrix_multiply_recursive(&a11, &b11),
            &square_matrix_multiply_recursive(&a12, &b21),
        );
        let c12 = square_matrix_add(
            &square_matrix_multiply_recursive(&a11, &b12),
            &square_matrix_multiply_recursive(&a12, &b22),
        );
        let c21 = square_matrix_add(
            &square_matrix_multiply_recursive(&a21, &b11),
            &square_matrix_multiply_recursive(&a22, &b21),
        );
        let c22 = square_matrix_add(
            &square_matrix_multiply_recursive(&a21, &b12),
            &square_matrix_multiply_recursive(&a22, &b22),
        );
        merge_four(&mut c, &c11, &c12, &c21, &c22);
    }
    c
}
```

其中 `partition_four()` 和 `merge_four()` 是将一个矩阵拆分成4个子矩阵以及将4个子矩阵合成一个矩阵的函数。

```rs
// partition_four()
pub fn partition_four<T: Clone>(
    a: &Vec<Vec<T>>,
    a11: &mut Vec<Vec<T>>,
    a12: &mut Vec<Vec<T>>,
    a21: &mut Vec<Vec<T>>,
    a22: &mut Vec<Vec<T>>,
) {
    let n = a.len();
    for i in 0..(n / 2) {
        for j in 0..(n / 2) {
            a11[i][j] = a[i][j].clone();
        }
    }
    for i in 0..(n / 2) {
        for j in (n / 2)..n {
            a12[i][j - n / 2] = a[i][j].clone();
        }
    }
    for i in (n / 2)..n {
        for j in 0..(n / 2) {
            a21[i - n / 2][j] = a[i][j].clone();
        }
    }
    for i in (n / 2)..n {
        for j in (n / 2)..n {
            a22[i - n / 2][j - n / 2] = a[i][j].clone();
        }
    }
}

// merge_four()
pub fn merge_four<T: Clone>(
    a: &mut Vec<Vec<T>>,
    a11: &Vec<Vec<T>>,
    a12: &Vec<Vec<T>>,
    a21: &Vec<Vec<T>>,
    a22: &Vec<Vec<T>>,
) {
    let n = a.len();
    for i in 0..(n / 2) {
        for j in 0..(n / 2) {
            a[i][j] = a11[i][j].clone();
        }
    }
    for i in 0..(n / 2) {
        for j in (n / 2)..n {
            a[i][j] = a12[i][j - n / 2].clone();
        }
    }
    for i in (n / 2)..n {
        for j in 0..(n / 2) {
            a[i][j] = a21[i - n / 2][j].clone();
        }
    }
    for i in (n / 2)..n {
        for j in (n / 2)..n {
            a[i][j] = a22[i - n / 2][j - n / 2].clone();
        }
    }
}
```

其实这部分的分解合并不是必要的，完全可以通过原矩阵的一组行下标和一组列下标来指明一个子矩阵，这里为了方便编码就不做进一步处理了。

对 $n = 1$ 的基本情况，只需进行一次标量乘法，时间复杂度为 $O(1)$。

当 $n > 1$ 时是递归情况，8次递归调用总时间为 $8T(n/2)$ ，矩阵加法总时间为 $\Theta(n^2)$，因此递归情况的总时间为分解时间、递归调用时间以及矩阵加法时间之和：

$$ T(n) = \Theta(1) + 8T(n / 2) + \Theta(n^2) = 8T(n / 2) + \Theta(n^2) $$

如果通过复制元素来实现矩阵分解，正如我们所做的那样，额外开销为 $\Theta(n^2)$，递归式不会发生改变，总运行时间会提高常数倍。

该算法的时间复杂度仍为 $O(n^3)$

## Strassen 方法

Strassen 算法的核心思想是令递归树稍微不那么茂盛一点，只递归进行7次而不是8次。

Strassen 算法的步骤：

1. 按公式将输入矩阵和输出矩阵分解为4个子矩阵，与算法2相同。

2. 创建 $10$ 个$n / 2 \times n /2$ 的矩阵 $S_0, S_1, ..., S_9$，每个矩阵保存步骤1中创建的两个子矩阵的和或差。

3. 用步骤1中创建的子矩阵和步骤2中创建的10个矩阵，递归计算7个矩阵积 $P_1, P_2, ..., P_7$，每个矩都是 $n / 2 \times n /2$。

4. 通过 $P_i$ 矩阵的不同组合进行加减运算，计算出矩阵 C 的子矩阵。

步骤2：

$$ S_0 = B_{12} - B_{22} $$

$$ S_1 = A_{11} + A_{12} $$

$$ S_2 = A_{21} + A_{22} $$

$$ S_3 = B_{21} - B_{11} $$

$$ S_4 = A_{11} + A_{22} $$

$$ S_5 = B_{11} + B_{22} $$

$$ S_6 = A_{12} - A_{22} $$

$$ S_7 = B_{21} + B_{22} $$

$$ S_8 = A_{11} - A_{21} $$

$$ S_9 = B_{11} + B_{12} $$

步骤3：

$$ P_1 = A_{11} \cdot S_0 $$

$$ P_2 = S_1 \cdot B_{22} $$

$$ P_3 = S_2 \cdot B_{11} $$

$$ P_4 = A_{22} \cdot S_3 $$

$$ P_5 = S_4 \cdot S_5 $$

$$ P_6 = S_6 \cdot S_7 $$

$$ P_7 = S_8 \cdot S_9 $$

步骤4：

$$ C_{11} = P_5 + P_4 - P_2 + P_6 $$

$$ C_{12} = P_2 + P_2 $$

$$ C_{21} = P_3 + P_4 $$

$$ C_{22} = P_5 + P_1 - P_3 - P_7 $$

写成代码即为：

```rs
// 算法3：Strassen算法
pub fn square_matrix_multiply_strassen<
    T: Default
        + Clone
        + AddAssign
        + SubAssign
        + Mul<Output = T>
        + Add<Output = T>
        + Sub<Output = T>
        + Debug,
>(
    a: &Vec<Vec<T>>,
    b: &Vec<Vec<T>>,
) -> Vec<Vec<T>>
where
    T: Mul<T> + Add<T> + Sub<T>,
{
    let mut n = a.len();
    let mut c = vec![vec![T::default(); n]; n];
    if n == 1 {
        c[0][0] = a[0][0].clone() * b[0][0].clone();
    } else {
        n /= 2;
        let mut a11 = vec![vec![T::default(); n]; n];
        let mut a12 = vec![vec![T::default(); n]; n];
        let mut a21 = vec![vec![T::default(); n]; n];
        let mut a22 = vec![vec![T::default(); n]; n];
        let mut b11 = vec![vec![T::default(); n]; n];
        let mut b12 = vec![vec![T::default(); n]; n];
        let mut b21 = vec![vec![T::default(); n]; n];
        let mut b22 = vec![vec![T::default(); n]; n];
        partition_four(&a, &mut a11, &mut a12, &mut a21, &mut a22);
        partition_four(&b, &mut b11, &mut b12, &mut b21, &mut b22);

        let s0 = square_matrix_sub(&b12, &b22);
        let s1 = square_matrix_add(&a11, &a12);
        let s2 = square_matrix_add(&a21, &a22);
        let s3 = square_matrix_sub(&b21, &b11);
        let s4 = square_matrix_add(&a11, &a22);
        let s5 = square_matrix_add(&b11, &b22);
        let s6 = square_matrix_sub(&a12, &a22);
        let s7 = square_matrix_add(&b21, &b22);
        let s8 = square_matrix_sub(&a11, &a21);
        let s9 = square_matrix_add(&b11, &b12);
        let p1 = square_matrix_multiply(&a11, &s0);
        let p2 = square_matrix_multiply(&s1, &b22);
        let p3 = square_matrix_multiply(&s2, &b11);
        let p4 = square_matrix_multiply(&a22, &s3);
        let p5 = square_matrix_multiply(&s4, &s5);
        let p6 = square_matrix_multiply(&s6, &s7);
        let p7 = square_matrix_multiply(&s8, &s9);

        let mut c11 = square_matrix_sub(&square_matrix_add(&p5, &p4), &square_matrix_sub(&p2, &p6));
        let mut c12 = square_matrix_add(&p1, &p2);
        let mut c21 = square_matrix_add(&p3, &p4);
        let mut c22 = square_matrix_sub(&square_matrix_add(&p5, &p1), &square_matrix_add(&p3, &p7));
        merge_four(&mut c, &mut c11, &mut c12, &mut c21, &mut c22);
    }
    c
}
```

时间复杂度$O(n^{lg7})$

Strassen 算法的渐进复杂性低于直接的定义法

编写测试函数测试正确性：

```rs
#[cfg(test)]
mod tests{
    #[test]
    fn test_recursive() {
        // 4 * 4 matrix
        let n = 4;
        // generate matrix a and b
        let mut a = vec![vec![0i64; n]; n];
        random_square_matrix_range(&mut a, n, (1, 10));
        let mut b = vec![vec![0i64; n]; n];
        random_square_matrix_range(&mut b, n, (1, 10));
        // calcu c by algo2
        let c = square_matrix_multiply_recursive(&a, &b);
        assert_eq!(c, square_matrix_multiply(&a, &b));
        // calcu c by algo3
        let c = square_matrix_multiply_strassen(&a, &b);
        assert_eq!(c, square_matrix_multiply(&a, &b));
        println!("{:?}", c);
    }
}
```

我们可以看到Strassen算法确实生成了正确的矩阵乘积。
