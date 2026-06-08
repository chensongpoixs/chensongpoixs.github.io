## 目录



1. [技术路线图](#技术路线图)
2. [业界痛点](#业界痛点)
3. [技术方案](#技术方案)
4. [项目路线图](#项目路线图)
5. [技术闭环](#技术闭环)


![](/img/llm_aigc/ai_base_ai_infra.png)
---

## 1. 技术路线图


从音视频、GPU 加速到大模型工程化，构建完整 AI 技术闭环。

![AI 技术闭环](/img/llm_aigc/tech-pipeline.svg)


---

## 2. 业界痛点



| 痛点             | 行业现状                             | 解决方案                                          |
| -------------- | -------------------------------- | --------------------------------------------- |
| 推理成本高          | 70B+ 模型依赖多卡集群，中小企业难以承担           | 通过知识蒸馏将能力迁移至 0.5B~7B 模型，成本降低 80%+             |
| 生成速度慢          | 大模型仅 20~50 Tokens/s，Agent 响应延迟明显 | 小模型 + vLLM + KV Cache 优化，可达到 150~500 Tokens/s |
| 部署门槛高          | 模型体积数百 GB，GPU 要求高                | INT4/AWQ/GPTQ 量化后单卡即可部署                       |
| 领域知识缺失         | 通用模型无法理解企业内部知识                   | RAG + 专业数据集微调构建领域专家模型                         |
| Agent 效果不稳定    | Tool Calling 容易失败                | ReAct + Workflow + MCP 提升执行成功率                |
| 数据获取困难         | 高质量 SFT 数据成本高                    | API 透明代理自动沉淀训练数据                              |
| 训练成本高          | 从零训练需要大量 GPU                     | 蒸馏 + LoRA 微调降低训练成本                            |
| 工程链路割裂         | 训练、推理、Agent 系统分散                 | 打通 Data → Train → Distill → Infer → Agent 全链路 |
| 私有化困难          | 数据无法出企业内网                        | 支持本地部署与离线推理                                   |
| 缺乏 AI Infra 能力 | 多数团队只会调用 API                     | 提供完整 AI 基础设施建设能力                              |


---

## 3. 技术方案

### 3.1 知识蒸馏 → 降成本

```text
Teacher (70B+)  →  Student (0.5B ~ 14B)
```

推理成本降低 10~50 倍，消费级 GPU 可部署。

> [PolyDistill](https://chensongpoixs.github.io/PolyDistill/)

### 3.2 领域模型 → 补知识

结合 FFmpeg / WebRTC / 流媒体 / GPU 加速积累，训练：

```text
AudioVideo-0.6B / 4B / 7B       Agent 专项蒸馏模型
```

### 3.3 Agent 平台 → 建智能体

<img src="/img/llm_aigc/agent-loop.svg" style="max-width: 260px; width: 100%;" alt="Agent 闭环" />

内容理解、智能处理、工具调用（FFmpeg / WebRTC / GPU 服务）自动协作。

### 3.4 推理优化 → 提速度

vLLM / TensorRT-LLM / SGLang · Continuous Batching · INT8/INT4 量化

提升 GPU 利用率与 Tokens/s，降低部署成本。

### 3.5 AI RTC → 落场景

<img src="/img/llm_aigc/ai-rtc-arch.svg" style="max-width: 320px; width: 100%;" alt="AI RTC 架构" />

AI 会议助手 · AI 客服 · AI 数字人

---

## 4. 项目路线图

| # | 项目 | 产出 |
|:---:|------|------|
| 1 | [**PolyDistill** 知识蒸馏平台](https://chensongpoixs.github.io/PolyDistill/) | 通用蒸馏框架，多架构 Teacher→Student |
| 2 | [**领域模型训练**]() | AudioVideo 系列、Agent 专项蒸馏模型 |
| 3 | [**音视频 Agent 平台**]() | 感知→思考→行动闭环，工具调用编排 |
| 4 | [**推理优化 & AI Infra**]() | 量化模型、高并发推理、GPU 资源优化 |
| 5 | [**AI RTC**]() | ASR+LLM+TTS+WebRTC 实时交互系统 |

---

## 5. 技术闭环

<img src="/img/llm_aigc/full-pipeline.svg" style="max-width: 380px; width: 100%;" alt="完整技术闭环" />

不追求最大模型，追求最低成本、最高效率、最易部署服务真实场景。
