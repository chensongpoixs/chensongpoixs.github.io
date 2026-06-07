# 陈松的技术博客

基于 Jekyll 4.3 构建的技术博客，托管在 GitHub Pages（chensongpoixs.github.io）。内容涵盖 C/C++、Linux 网络编程、GPU/CUDA、视频编解码、WebRTC、OpenGL 和 AI/ML 等技术方向。

## 本地运行

### 前置要求

- Ruby 3.1+（推荐 rbenv 安装 Ruby 3.2.2）
- Bundler

### 快速启动

```bash
# 安装依赖
bundle install

# 启动本地开发服务器
bundle exec jekyll serve

# 或者监听文件变化
bundle exec jekyll serve --watch

# 允许局域网访问
bundle exec jekyll serve --host 0.0.0.0
```

浏览器访问 http://localhost:4000

### 其他命令 

```bash
bundle exec jekyll build        # 构建生产版本到 _site/
bundle exec jekyll clean        # 清理生成文件
grunt                           # 前端资源构建（LESS → CSS, JS 压缩）
grunt watch                     # 监听模式
```

## 项目架构

### 布局模板（_layouts/）

| 模板 | 说明 |
|------|------|
| `default` | 基础 HTML 骨架，引入 head、nav、footer |
| `page` | 标准页面布局，支持三栏模式（左侧：前沿技术+创作历程，中间：内容，右侧：标签+关于+友链） |
| `post` | 博客文章页，含文章头图、标签链接、目录导航、Gitalk/Disqus 评论、上下页导航 |
| `keynote` | 类似 post，但文章头部支持全屏 iframe 嵌入内容 |

### 页面类型

- **根页面**（顶层 `.html`）：`index.html`（首页分页文章列表）、`about.html`、`tags.html`、`Cuda.html`、`OpenGL.html`、`WebRTC.html`、`artificial_intelligence.html`、`video_codec.html`
- **双语主题页**：Cuda、OpenGL、WebRTC、AI、video_codec、about 等页面采用中英双语切换模式，中文内容在 `_includes/<topic>/zh.md`，英文在 `_includes/<topic>/en.md`，语言切换 JS 在 `footer.html` 中
- **博客文章**：Markdown 文件存放在 `_posts/` 目录，每页显示 10 篇（jekyll-paginate）

### 关键功能模块

| 模块 | 位置 | 说明 |
|------|------|------|
| 导航栏 | `_includes/nav.html` | CSS3 动画导航，无 jQuery 依赖 |
| SEO 头部 | `_includes/head.html` | meta 标签、CSS、Prism.js 代码高亮、MathJax 公式 |
| 页脚 | `_includes/footer.html` | 社交链接、GA/Baidu 统计、搜索浮层、目录生成 |
| 前沿技术 | `_includes/frontier-tech.html` | 从 `_config.yml` 的 `frontier_techs` 配置渲染技术卡片 |
| 标签云 | `_includes/featured-tags.html` | 首页标签展示 |
| 评论系统 | Gitalk（主）+ Disqus（备用） | 配置在 `_config.yml` 的 `gitalk` 和 `disqus_username` |
| 站内搜索 | `search/` 目录 | 基于 bootstrap3-typeahead |
| PWA | `sw.js` | Service Worker 缓存策略，支持离线访问 |

### 前端资源

```
less/               → Grunt → css/hux-blog.css
js/                 → Grunt → js/*.min.js
```

Gruntfile.js 同时包含一个 Snow/Snows 类（雪花/雨滴动画效果），与构建管线无关。

## ⚠️ 关键注意事项

### tags 缩进必须用空格，不能用 Tab

在 `_posts/*.md` 的 frontmatter 中，`tags` 列表的缩进**只能使用空格**，使用 Tab 键会导致 Jekyll 解析失败，页面 404。

正确：

```yaml
tags:
    - 网络编程
```

错误（使用了 Tab）：

```yaml
tags:
	- 网络编程
```

### disqus_username 的值

务必使用 `chensongpoixs-github-io-1`，而不是 `chensongpoixs`，否则 Disqus 加载失败。

Disqus 的 shortname 是在 Disqus 管理后台配置的，不是 GitHub 用户名。

## 文章 Frontmatter 参考

```yaml
---
layout:     post
title:      网络协议详解
subtitle:   ARP协议，DNS协议，IP协议，TCP协议，IP路由和TCP状态转换图
date:       2018-02-12
times:      23::09::56                   # 可选，发布时间戳
author:     chensong
header-img: img/2018-02-12/bg_tcp.jpg    # 文章头图（可选）
catalog:    true                          # 是否显示侧边目录（可选）
tags:
    - TCP/IP协议详解
---
```

## 站点配置

`_config.yml` 中的关键配置项：

- `frontier_techs` — "博主前沿技术"区域展示的技术卡片
- `friends` — 友情链接列表
- `featured-condition-size` — 相同标签达到这个数量才在首页展示
- `sidebar` — 是否启用三栏布局
- `paginate: 10` — 首页每页显示的文章数量
