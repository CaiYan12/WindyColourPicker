# Windy Colour Picker 🎨

A lightweight, Fluent-styled colour picker for Windows built with Qt 6.

![Qt 6](https://img.shields.io/badge/Qt-6.10.0-41CD52?logo=qt)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D4?logo=windows)
![License](https://img.shields.io/badge/License-MIT-blue)

## Features

- **HSV Colour Wheel** — Intuitive drag-to-pick colour selection
- **Screen Colour Picking** — Hover anywhere on screen to pick colours in real time (Win32 low-level mouse hook)
- **Multiple Formats** — Display and copy HEX, RGB, RGBA, HSV values
- **Brightness & Alpha Sliders** — Fine-tune value and transparency
- **Floating Tooltip** — Live colour preview follows cursor while picking
- **Fluent Design** — Clean, modern UI inspired by Microsoft Fluent Design

## Screenshots

> TODO: Add screenshots

## Requirements

- Qt 6.10.0+ (MSVC 2022 64-bit)
- Visual Studio 2022 (MSVC v143)
- Windows 10/11

## Build

```bat
# Option 1: Use the bundled build script
build.bat

# Option 2: Manual build
qmake WindyColourPicker.pro -spec win32-msvc "CONFIG+=release"
nmake
cd release
windeployqt WindyColourPicker.exe
```

## Usage

1. Launch `WindyColourPicker.exe`
2. Click **🎯 开启取色模式** to start picking colours from screen
3. Move cursor to the desired colour — the tooltip shows a live preview
4. Press **ESC** or click the button again to stop picking
5. Adjust brightness and alpha using the sliders
6. Click any colour value field to copy it to clipboard

## Project Structure

```
WindyColourPicker/
├── main.cpp                 # Application entry point
├── WindyColourPicker.cpp/h  # Main window & screen pick logic
├── ColorWheel.cpp/h         # HSV colour wheel widget
├── ColorTipWidget.cpp/h     # Floating colour tooltip
├── WindyColourPicker.pro    # Qt project file
└── build.bat                # One-click build script
```

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.
