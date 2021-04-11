# 各种情况下计算的gflops

## benchmark

本地x86_64测试，cpu型号(Intel(R Core(TM) i5-9600K base@3.7GHz, turbo@4.6GHz)
使用[大佬](https://zhuanlan.zhihu.com/p/28226956)的benchmark测试方案，[github指路](https://github.com/pigirons/cpufp)测试后结果得到如下：

Thread(s): 1
fma fp32 perf: 145.7040 gflops.
fma fp64 perf: 71.1179 gflops.
avx fp32 perf: 71.2300 gflops.
avx fp64 perf: 35.9344 gflops.
sse fp32 perf: 36.4025 gflops.
sse fp64 perf: 18.1609 gflops.

Thread(s): 4
fma fp32 perf: 552.8332 gflops.
fma fp64 perf: 273.2259 gflops.
avx fp32 perf: 275.1136 gflops.
avx fp64 perf: 136.6069 gflops.
sse fp32 perf: 137.5296 gflops.
sse fp64 perf: 68.6335 gflops.

## 各优化版本

### MMult0

- 此版本是一个最原始的naive矩阵乘实现，未考虑任何优化策略，只有简单的矩阵乘ijk三重循环。

### MMult1

- 通过将最内侧循环写到一个函数中，通过figure_1可以看到基本没有任何提升，此时的浮点峰值大约在3gflops左右。

### MMult2

- 第二重循环以步长为4来进行，最内侧有四个位置的元素被统一计算。但是根据数据来看结果仍然没有大幅度的提升。

### MMult_1x4_3

- 将四次计算一个元素值的函数放在一个AddDot1x4函数中来调用，实际上没有任何实质性的提升，性能还是在3gflops左右。

### MMult_1x4_4

- 将函数调用在AddDot1x4的函数内部展开，减少函数调用开销？如图片比较所示，并没有什么提升。

### MMult_1x4_5

- 将函数AddDot1x4内部的函数调用展开后所有元素的计算放在一个循环中，减少p更新的次数。此时性能有了一定幅度上的提升，最大性能可以达到上一个版本的2倍左右。（不过与参考教程里面的测试情况不太吻合，详细原因目前还不太清楚。）经实际测试，发现时当前矩阵不够大(当前最大是800x800，double类型数据，总数据量是4.88M左右，本机L1 cache（指令cache和data cache）大小为32768Byte，L2 cache大小是262144byte,约0.25M；L3 cache大小是9437184byte，为9M)，因此猜测是当前矩阵不够大，当矩阵最大扩大到1600x1600时，可见指标明显下降。见图Figure_MMult_1x4_5_vs_MMult_1x4_6_2.png。

### MMult_1x4_6

- 结果矩阵C和矩阵A变量存储使用寄存器，减少cache和寄存器之间的交互。根据Figure_MMult_1x4_5_vs_MMult_1x4_6.png所示，有一定程度上的提升，，最高时的浮点计算能力可以达到8gflops左右。

### MMult_1x4_7

- 在MMult_1x4_6的基础上，使用指针来访问矩阵B，避免使用索引产生的开销，相对来说还有一定程度上的提升。

### MMult_1x4_8

- 在上一版本的优化基础上，增加了循环展开的，最内侧计算时一次计算四个，对比图片发现没有较大提升。

### MMult_1x4_9

- 通过指针加减间接寻址，避免指针的频繁更新，经过测试，发现也没有大幅度的提升。

### MMult_4x4_3

- 一次计算16个元素，没有明显性能提升

### MMult_4x4_4

- 将函数调用展开，每个元素的计算做一次循环，与上个版本也没有明显变化。

### MMult_4x4_5

- 将所有的循环放在一起，减少指针p的更新（此时已经有gflops相比上个版本已经有一定程度的提升，峰值从3gflops到5gflops左右）

### MMult_4x4_6

- 将C矩阵中的元素和A矩阵中的元素放在寄存器中，此时性能会有较大提升(浮点峰值从4gflops到8gflops左右)

### MMult_4x4_7

- 使用指针寻址矩阵B中的元素（性能基本无变化）

### MMult_4x4_8

- 将B中的元素也放到寄存器中（无变化）

### MMult_4x4_9

- 重新排计算的流程，与上个版本只有很小的变化，为了下一个版本中向量寄存器的使用做准备。（实际上并没有变化）

### MMult_4x4_10

- 在此优化版本中用到了向量寄存器，一个128位向量寄存器可以存储2各双精度浮点数，或者存储4各单精度浮点数。
- SSE(Streaming SIMD Extensions): 8个128为寄存器（从xmm0到xmm7)
- 数据类型：__m128 / __m128i / __m128d分别表示单精度浮点数，整型和双精度浮点数。
- 此时的gflops已经有较大的提升（最高从8gflops到了12gflops）

### MMult_4x4_11 and MMult_4x4_12

- 按照pack的策略分别对矩阵A和矩阵B进行了重排，提升局部性，在800x800的矩阵大小下发现效果并不是特别明显。

### To be continued

当前性能最高可以达到12gflops，但实际测量的浮点峰值在72gflops（fp64）左右，约达到了实际效果的1/6，后续还有较大的优化空间


## Reference

- https://github.com/flame/how-to-optimize-gemm
- https://github.com/tpoisonooo/how-to-optimize-gemm
- https://github.com/BBuf/how-to-optimize-gemm
- https://zhuanlan.zhihu.com/p/28226956
- https://zhuanlan.zhihu.com/p/65436463