# ccuda_sample


![CUDA/GPU](/img/cuda_gpu_relation.png)

图标设计说明：

- GPU（硬件层）：底层绿色矩形，表示硬件基础，包含多个核心方块
- CUDA（编程模型层）：中间蓝色矩形，表示 NVIDIA 的编程平台
- 并行计算：左侧橙色圆形，表示并行计算应用
- HPC（高性能计算）：右侧紫色圆形，表示高性能计算应用
- 连接关系：箭头表示从 GPU → CUDA → 应用层的层次关系
- 网格背景：表示并行计算的网格结构

CUDA   流处理器，核，线程块（threadblock），线程，网格（‌gridDim），块（block） 、 OpenGL 