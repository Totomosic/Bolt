# Bolt
![Build-Windows](https://github.com/Totomosic/Bolt/workflows/Build-Windows/badge.svg)
![Build-Linux](https://github.com/Totomosic/Bolt/workflows/Build-Linux/badge.svg)

3D/2D C++ OpenGL Graphics/Game Engine.

## Supported Platforms:
![](https://img.shields.io/badge/Windows-Supported-green.svg)

![](https://img.shields.io/badge/Linux-Supported-green.svg)

![](https://img.shields.io/badge/MacOS-Not%20Supported-red.svg)

![](https://img.shields.io/badge/Android-Not%20Supported-red.svg)

![](https://img.shields.io/badge/IOS-Not%20Supported-red.svg)

Currently supports both Windows and Linux, untested on MacOS and plans to support Android/ios in the future.

## Installing:
1. Download or clone this repository.
2. If on windows run the `Scripts/Win-GenProjects.bat` script to generate the Visual Studio 2019 project and solution files.

## Building on Windows:
1. Run `Scripts/Win-GenProjects.bat` and build the Bolt-Core project using Visual Studio 2019.
2. Build outputs are located in the `bin` directory.

## Building on Linux:
1. Ensure the required dependencies are installed `sudo apt-get install libx11-dev libgl1-mesa-dev xorg-dev`
2. Run `Scripts/Linux-GenProjects.sh` to generate the Makefiles.
3. Run `make -j<number_of_cores> Bolt-Core` to build the core library.
4. Build outputs are located in the `bin` directory.
