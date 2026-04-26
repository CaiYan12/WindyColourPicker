# Windy Colour Picker 🎨

一个基于 Qt 6 的轻量级 Windows 取色器，采用 Fluent 风格界面。

![Qt 6](https://img.shields.io/badge/Qt-6.10.0-41CD52?logo=qt)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D4?logo=windows)
![License](https://img.shields.io/badge/License-MIT-blue)

## 功能

- **HSV 色盘** — 直观的拖拽式颜色选择
- **屏幕取色** — 鼠标悬停即可实时拾取屏幕任意位置的颜色（Win32 底层鼠标钩子）
- **多格式显示** — 支持 HEX、RGB、RGBA、HSV，点击即复制
- **明度 & 透明度滑块** — 精细调节色彩明暗与透明通道
- **浮动提示** — 取色模式下实时跟随光标的颜色预览
- **Fluent 风格** — 简洁现代的界面设计

## 截图

> TODO: 添加截图

## 环境要求

- Qt 6.10.0+（MSVC 2022 64-bit）
- Visual Studio 2022（MSVC v143）
- Windows 10 / 11
- VC++ 2022 x64 运行时

## 构建

```bat
:: 方式一：使用构建脚本
build.bat

:: 方式二：手动构建
qmake WindyColourPicker.pro -spec win32-msvc "CONFIG+=release"
nmake
cd release
windeployqt WindyColourPicker.exe
```

## 使用

1. 运行 `WindyColourPicker.exe`
2. 点击 **🎯 开启取色模式** 进入屏幕取色
3. 移动光标到目标颜色处，浮动提示实时预览
4. 按 **ESC** 或再次点击按钮停止取色
5. 拖动滑块调节明度和透明度
6. 点击颜色值字段即可复制到剪贴板

## 项目结构

```
WindyColourPicker/
├── main.cpp                 # 程序入口
├── WindyColourPicker.cpp/h  # 主窗口与屏幕取色逻辑
├── ColorWheel.cpp/h         # HSV 色盘控件
├── ColorTipWidget.cpp/h     # 浮动颜色提示
├── WindyColourPicker.pro    # Qt 工程文件
└── build.bat                # 一键构建脚本
```

## 许可证

[MIT License](LICENSE)
