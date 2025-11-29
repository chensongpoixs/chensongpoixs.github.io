@echo off
chcp 65001 >nul
echo ========================================
echo    Jekyll 博客本地服务器启动脚本
echo ========================================
echo.

REM 检查 Ruby 是否安装
where ruby >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未检测到 Ruby！
    echo.
    echo 请先安装 Ruby：
    echo 1. 访问 https://rubyinstaller.org/downloads/
    echo 2. 下载并安装 Ruby+Devkit
    echo 3. 确保勾选 "Add Ruby executables to your PATH"
    echo 4. 重启命令行窗口后再次运行此脚本
    echo.
    pause
    exit /b 1
)

echo [信息] 检测到 Ruby 已安装
ruby -v
echo.

REM 检查 Bundler 是否安装
where bundle >nul 2>&1
if %errorlevel% neq 0 (
    echo [信息] 正在安装 Bundler...
    gem install bundler
    if %errorlevel% neq 0 (
        echo [错误] Bundler 安装失败！
        echo 请以管理员身份运行此脚本
        pause
        exit /b 1
    )
)

echo [信息] 检查依赖...
bundle check >nul 2>&1
if %errorlevel% neq 0 (
    echo [信息] 正在安装项目依赖...
    bundle install
    if %errorlevel% neq 0 (
        echo [错误] 依赖安装失败！
        pause
        exit /b 1
    )
)

echo.
echo ========================================
echo    正在启动 Jekyll 服务器...
echo ========================================
echo.
echo 服务器启动后，请在浏览器访问：
echo http://localhost:4000
echo.
echo 按 Ctrl+C 停止服务器
echo.

REM 启动 Jekyll 服务器
bundle exec jekyll serve --watch

pause

