## 目录

1. [技术路线图](#技术路线图)
2. [业界痛点](#业界痛点)
3. [技术方案](#技术方案)
4. [项目路线图](#项目路线图)
5. [技术闭环](#技术闭环)

---

## 1. 技术路线图

从音视频、GPU 加速到大模型工程化，构建完整 AI 技术闭环。

![AI 技术闭环](/img/llm_aigc/tech-pipeline.svg)

---

## 2. 业界痛点

| 痛点 | 现状 |
|------|------|
| **推理成本高** | 70B+ 模型需多卡集群，中小企业无法承受 |
| **生成速度慢** | 大模型 20~50 Tokens/s，Agent 场景无法实时响应 |
| **部署门槛高** | 模型数百 GB，私有化部署困难 |
| **领域知识缺** | 通用模型不懂音视频、GPU 加速等垂直领域 |
| **工程链路散** | 蒸馏、训练、推理、Agent 各自为战，集成成本高 |

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
| 1 | **PolyDistill** 知识蒸馏平台 | 通用蒸馏框架，多架构 Teacher→Student |
| 2 | **领域模型训练** | AudioVideo 系列、Agent 专项蒸馏模型 |
| 3 | **音视频 Agent 平台** | 感知→思考→行动闭环，工具调用编排 |
| 4 | **推理优化 & AI Infra** | 量化模型、高并发推理、GPU 资源优化 |
| 5 | **AI RTC** | ASR+LLM+TTS+WebRTC 实时交互系统 |

---

## 5. 技术闭环

<img src="/img/llm_aigc/full-pipeline.svg" style="max-width: 380px; width: 100%;" alt="完整技术闭环" />

不追求最大模型，追求最低成本、最高效率、最易部署服务真实场景。
