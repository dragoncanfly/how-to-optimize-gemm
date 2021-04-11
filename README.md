# 如何一步一步优化矩阵乘
参考 github.com/flame/how-to-optimize-gemm 学习优化矩阵乘
## 实验环境

Intel i5 @3.7GHz

## 结果

- benchmark: fp64 71.12 gflops
- 当前最优： fp64 10.56 gflops (20210411)
- 当前只达到峰值计算能力的1/7，仍存在很大的优化空间（to be continued）
- 详细内容可见[这里](https://github.com/dragoncanfly/how-to-optimize-gemm/blob/master/doc/test.md)
