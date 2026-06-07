## Contents

1. [Technology Roadmap](#1-technology-roadmap)
2. [Industry Pain Points](#2-industry-pain-points)
3. [Technical Approach](#3-technical-approach)
4. [Project Roadmap](#4-project-roadmap)
5. [Technology Closed Loop](#5-technology-closed-loop)

---

## 1. Technology Roadmap

From audio/video and GPU acceleration to LLM engineering — a complete AI technology closed loop.

![AI Technology Closed Loop](/img/llm_aigc/tech-pipeline.svg)

---

## 2. Industry Pain Points

| Pain Point | Reality |
|------|------|
| **High Inference Cost** | 70B+ models require multi-GPU clusters — prohibitive for most |
| **Slow Token Throughput** | 20~50 Tokens/s on large models — too slow for real-time Agent |
| **High Deployment Barrier** | Hundreds of GB per model — private deployment is hard |
| **Missing Domain Knowledge** | General models lack expertise in audio/video, GPU acceleration |
| **Fragmented Pipeline** | Distillation, training, inference, Agent — siloed, costly to integrate |

---

## 3. Technical Approach

### 3.1 Knowledge Distillation → Cut Cost

```text
Teacher (70B+)  →  Student (0.5B ~ 14B)
```

Inference cost reduced 10~50×. Deployable on consumer GPUs.

> [PolyDistill](https://chensongpoixs.github.io/PolyDistill/)

### 3.2 Domain Models → Fill Knowledge Gaps

Leveraging FFmpeg / WebRTC / streaming / GPU acceleration expertise:

```text
AudioVideo-0.6B / 4B / 7B       Agent-specific distilled models
```

### 3.3 Agent Platform → Build Intelligence

<img src="/img/llm_aigc/agent-loop.svg" style="max-width: 260px; width: 100%;" alt="Agent Loop" />

Content understanding, intelligent processing, tool invocation (FFmpeg / WebRTC / GPU services).

### 3.4 Inference Optimization → Boost Speed

vLLM / TensorRT-LLM / SGLang · Continuous Batching · INT8/INT4 quantization

Higher GPU utilization & Tokens/s, lower deployment cost.

### 3.5 AI RTC → Real Applications

<img src="/img/llm_aigc/ai-rtc-arch.svg" style="max-width: 320px; width: 100%;" alt="AI RTC Architecture" />

AI Meeting Assistant · AI Customer Service · AI Digital Human

---

## 4. Project Roadmap

| # | Project | Deliverables |
|:---:|------|------|
| 1 | **PolyDistill** Knowledge Distillation | Universal framework, multi-architecture Teacher→Student |
| 2 | **Domain Model Training** | AudioVideo series, Agent-specific distilled models |
| 3 | **Audio/Video Agent Platform** | Perception→Reasoning→Action loop, tool orchestration |
| 4 | **Inference Optimization & AI Infra** | Quantized models, high-concurrency, GPU optimization |
| 5 | **AI RTC** | ASR+LLM+TTS+WebRTC real-time interactive system |

---

## 5. Technology Closed Loop

<img src="/img/llm_aigc/full-pipeline.svg" style="max-width: 380px; width: 100%;" alt="Complete Technology Closed Loop" />

Not the largest model — the lowest cost, highest efficiency, easiest deployment for real-world impact.
