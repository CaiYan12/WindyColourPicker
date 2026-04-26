@echo off
chcp 65001 >nul
REM WindyColourPicker 构建后清理脚本
REM 删除 release 目录中的编译中间文件和不必要的运行时依赖

set TARGET=%~dp0build\release

if not exist "%TARGET%\WindyColourPicker.exe" (
    echo [清理] release 目录不存在，跳过
    exit /b 0
)

echo [清理] 正在清理 release 目录...

REM 删除编译中间文件
del /q "%TARGET%\*.obj" 2>nul
del /q "%TARGET%\*.res" 2>nul
del /q "%TARGET%\moc_*.cpp" 2>nul
del /q "%TARGET%\qrc_*.cpp" 2>nul
del /q "%TARGET%\moc_predefs.h" 2>nul

REM 删除不必要的运行时
del /q "%TARGET%\vc_redist.x64.exe" 2>nul
del /q "%TARGET%\dxcompiler.dll" 2>nul
del /q "%TARGET%\dxil.dll" 2>nul
del /q "%TARGET%\Qt6Network.dll" 2>nul
del /q "%TARGET%\Qt6Svg.dll" 2>nul

REM 删除不必要的插件目录
if exist "%TARGET%\generic" rd /s /q "%TARGET%\generic"
if exist "%TARGET%\tls" rd /s /q "%TARGET%\tls"
if exist "%TARGET%\networkinformation" rd /s /q "%TARGET%\networkinformation"

REM 删除依赖已移除的插件
del /q "%TARGET%\iconengines\qsvgicon.dll" 2>nul
del /q "%TARGET%\imageformats\qsvg.dll" 2>nul

echo [清理] 完成