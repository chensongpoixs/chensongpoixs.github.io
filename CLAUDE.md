# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a Jekyll 4.3 technical blog hosted on GitHub Pages (chensongpoixs.github.io). Content is primarily about C/C++, Linux networking, GPU/CUDA, video codecs, WebRTC, OpenGL, and AI/ML.

## Essential Commands

```bash
# Install dependencies
bundle install

# Start local dev server (http://localhost:4000)
bundle exec jekyll serve

# Start with auto-rebuild on changes
bundle exec jekyll serve --watch

# Build for production (output in _site/)
bundle exec jekyll build

# Clean generated files
bundle exec jekyll clean

# Frontend asset pipeline (LESS → CSS, JS minification)
grunt                    # Runs uglify + less + banner
grunt watch              # Watch mode for LESS/JS changes
```

There is no test suite for this project.

## Architecture

### Layout hierarchy

All pages use one of four layouts (in `_layouts/`):

- **`default`** — Base HTML shell. Includes `head.html`, `nav.html`, `footer.html` in that order. This is the root layout that everything inherits from.
- **`page`** — Extends `default`. Used for standalone pages (index, about, topic pages). Supports a 3-column layout when `site.sidebar` is true: left sidebar (frontier-tech + creation-timeline), center (content), right sidebar (featured-tags + about + friends links).
- **`post`** — Extends `default`. Used for `_posts` entries. Includes article header with background image, tag links, catalog/sidebar navigation generated from headings, Disqus + Gitalk comments, and previous/next post pagination.
- **`keynote`** — Extends `default`. Like `post` but with a fullscreen iframe header (used for embedded content).

### Page types

**Root pages** (top-level `.html` files): `index.html` (paginated post list), `about.html`, `tags.html`, `Cuda.html`, `OpenGL.html`, `WebRTC.html`, `artificial_intelligence.html`, `video_codec.html`, `plan.html`, `offline.html`, `404.html`, `url.html`, `rtc.html`.

**Topic pages** (Cuda, OpenGL, WebRTC, AI, video_codec, about, plan) follow a bilingual pattern: they set `multilingual: true` in frontmatter, then include `_includes/<topic>/zh.md` and `_includes/<topic>/en.md` for Chinese/English versions. Language switching is handled by JavaScript in `footer.html`.

**Blog posts** live in `_posts/` as Markdown files with YAML frontmatter. Jekyll-paginate displays 10 posts per page on the index.

### Key includes

- `head.html` — SEO meta tags, CSS includes (bootstrap, hux-blog, syntax highlighting, tech-enhancement), Prism.js for code highlighting, MathJax for LaTeX
- `nav.html` — Fixed-top navbar with custom CSS3 animation (no Bootstrap JS dependency)
- `footer.html` — Social links, copyright, Google/Baidu analytics, service worker registration, search overlay, catalog generation JS, multi-language toggle JS
- `frontier-tech.html` — "博主前沿技术" card grid rendered from `site.frontier_techs` config
- `featured-tags.html` / `featured_post_tags.html` — Tag cloud widgets

### CSS/JS build pipeline

Grunt (Gruntfile.js) processes:
- `less/` → `css/hux-blog.css` and `css/hux-blog.min.css`
- `js/chensongpoixs.github.io.js` → `js/chensongpoixs.github.io.min.js`

The Gruntfile also contains a `Snow`/`Snows` class implementation (rain/snow animation effect) that is unrelated to the build pipeline.

### Comment & Analytics systems

- **Gitalk** (active) — GitHub issue-based comments, configured via `site.gitalk` in `_config.yml`
- **Disqus** (fallback) — configured via `site.disqus_username`
- **Google Analytics** — `site.ga_track_id`
- **Baidu Tongji** — `site.ba_track_id`

### PWA

Service worker (`sw.js`) provides offline support with stale-while-revalidate caching strategy and automatic redirect fix for GitHub Pages 404s on path-without-trailing-slash URLs.

## Critical Pitfalls

### YAML frontmatter: tags must use spaces, NOT tabs

In `_posts/*.md` frontmatter, tags MUST be indented with spaces, never tabs. Using a tab character in the `tags:` list causes Jekyll to silently fail to generate the page.

Correct:
```yaml
tags:
    - 网络编程
```

Wrong (tab-indented — page will 404):
```yaml
tags:
	- 网络编程
```

### disqus_username value

Must be `chensongpoixs-github-io-1`, not `chensongpoixs`. Using the wrong value causes Disqus to fail to load.

## Site Configuration

All site-level settings are in `_config.yml`, including:
- `frontier_techs` — array of tech cards displayed in the left sidebar and on the index page
- `friends` — blogroll links
- `featured-tags` / `featured-condition-size` — tag display threshold
- `sidebar` — toggles the 3-column vs single-column layout on `page` layout
