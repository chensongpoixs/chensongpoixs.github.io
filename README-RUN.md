# 本地运行 Jekyll 博客

## 前置要求

1. **安装 Ruby** (Windows)
   - 下载并安装 RubyInstaller: https://rubyinstaller.org/
   - 推荐版本: Ruby+Devkit 3.1.x 或更高版本
   - 安装时勾选 "Add Ruby executables to your PATH"

2. **安装 Jekyll**
   ```bash
   gem install jekyll bundler
   ```

## 运行步骤

### 方法一：使用 Bundler（推荐）

1. **安装依赖**
   ```bash
   bundle install
   ```

2. **启动本地服务器**
   ```bash
   bundle exec jekyll serve
   ```
   或者
   ```bash
   bundle exec jekyll serve --watch
   ```

3. **访问博客**
   - 打开浏览器访问: http://localhost:4000
   - 或者: http://127.0.0.1:4000

### 方法二：直接使用 Jekyll

1. **安装 Jekyll（如果还没安装）**
   ```bash
   gem install jekyll jekyll-paginate jekyll-sitemap
   ```

2. **启动服务器**
   ```bash
   jekyll serve
   ```
   或者
   ```bash
   jekyll serve --watch
   ```

3. **访问博客**
   - http://localhost:4000

## 常用命令

- `jekyll serve` - 启动开发服务器
- `jekyll serve --watch` - 启动服务器并监听文件变化
- `jekyll serve --host 0.0.0.0` - 允许局域网访问
- `jekyll build` - 构建静态网站到 `_site` 目录
- `jekyll clean` - 清理生成的文件

## 故障排除

### 问题1: 找不到 jekyll 命令
**解决方案**: 确保 Ruby 已正确安装并添加到 PATH

### 问题2: 依赖安装失败
**解决方案**: 
```bash
gem install bundler
bundle install
```

### 问题3: Windows 上 wdm 错误
**解决方案**: 已包含在 Gemfile 中，运行 `bundle install` 即可

### 问题4: 端口被占用
**解决方案**: 使用其他端口
```bash
jekyll serve --port 4001
```

## 开发模式

如果需要实时预览更改，使用：
```bash
jekyll serve --watch --livereload
```

## 构建生产版本

```bash
jekyll build
```
生成的文件在 `_site` 目录中。

