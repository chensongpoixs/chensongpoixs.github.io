![](/img/llm_aigc/ai_base_ai_infra_en.svg)

## 1. Technology Roadmap

From audio/video and GPU acceleration to LLM engineering — a complete AI technology closed loop.

![AI Technology Closed Loop](/img/llm_aigc/tech-pipeline.svg)

---

## 2. Industry Pain Points
 

| Pain Point | Industry Status | Solution |
|------------|----------------|----------|
| High inference cost | 70B+ models rely on multi‑GPU clusters, unaffordable for SMEs | Transfer capabilities to 0.5B–7B models via knowledge distillation → cost reduced by 80%+ |
| Slow generation speed | Large models only 20–50 Tokens/s, noticeable Agent response latency | Small model + vLLM + KV Cache optimization → achieves 150–500 Tokens/s |
| High deployment barrier | Model size hundreds of GB, high GPU requirements | INT4 / AWQ / GPTQ quantization → single‑GPU deployment |
| Lack of domain knowledge | Generic models cannot understand enterprise internal knowledge | RAG + fine‑tuning on domain datasets → build domain expert model |
| Unstable Agent performance | Tool Calling often fails | ReAct + Workflow + MCP → improve execution success rate |
| Difficulty in data acquisition | High cost of high‑quality SFT data | API transparent proxy → automatically accumulate training data |
| High training cost | Full training requires massive GPU resources | Distillation + LoRA fine‑tuning → reduce training cost |
| Fragmented engineering pipeline | Training, inference, Agent systems are siloed | Unify the full pipeline: Data → Train → Distill → Infer → Agent |
| Difficulty in private deployment | Data cannot leave the corporate intranet | Support local deployment and offline inference |
| Lack of AI Infra capability | Most teams only know how to call APIs | Provide complete AI infrastructure building capability |

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
