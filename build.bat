@echo off
chcp 65001 >nul
echo [1/5] 初始化 VS2022 环境...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

echo [2/5] 设置 Qt 路径...
set PATH=C:\Qt\6.10.0\msvc2022_64\bin;%PATH%

echo [3/5] 生成 Makefile...
if not exist build mkdir build
cd build
qmake ..\WindyColourPicker.pro -spec win32-msvc "CONFIG+=release"

echo [4/5] 编译工程...
nmake

echo [5/5] 部署 Qt 依赖库...
cd release
windeployqt WindyColourPicker.exe --no-translations --no-system-d3d-compiler --no-opengl-sw

echo [6/6] 清理中间文件...
call "%~dp0clean_release.bat"

echo.
echo ========================================
echo 构建完成！
echo 可执行文件位置: build\release\WindyColourPicker.exe
echo ========================================
echo.
