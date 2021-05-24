# *GestureControl*
Windows application for control multimedia system via gesture recognition

## Table of contents
0. [Table of contents](#table-of-contents)
1. [System requirements](#system-requirements)
2. [How to run?](#how-to-run)
3. [How to use?](#how-to-use)
4. [Gestures](#gestures)
4. [Software requirements for development](#software-requirements-for-development)
5. [How to build?](#how-to-build)

## System requirements
* Microsoft Windows 10 x64 version 20H2
* About 100 MiB of RAM
* About 100 MiB of physical memory
* Webcam with minimal resolution 640*480

## How to run?
To run version for Windows just execute file "bin/\<Configuration\>/GestureControl.exe".  
In the Debug version there will be a terminal with some debug information and a window with web-camera realtime video.  
In the Release version there wiil be only a tray icon.

## How to use?
Run the application ([How to run?](#how-to-run)). Once your web-camera has started capturing a video, you can show a one of [pretrained gestures](#gestures) to control Windows multimedia system.
Using a tray icon you can add or remove an application from autorun, pause or resume gestures recognition, close the application.

## Gestures
To control Windows multimedia system you can use one of following gestures:

* Fist - Stop
* Palm - Play/Pause
* Thump up - Increase volume
* Thump down - Decrease volume
* Spok - Pause recognition (manual resuming via tray icon is required)

## Software requirements for development
* OS:
    * Microsoft Windows 10 x64 version 20H2
* Git 2.25.1
* Microsoft Visual Studio 2019 16.9.2
* Anaconda 3 2020.07 with Python 3.8.3 64-bit
* C++ libraries:
    * OpenCV 4.5.2 for VC14-15
    * Eigen 3.3.9
    * Frugally-deep 0.15.2-p0 [[github]](https://github.com/Dobiasd/frugally-deep)
    * FunctionalPlus 0.2.14-p0 [[github]](https://github.com/Dobiasd/FunctionalPlus)
    * JSON for Modern C++ 3.9.1 [[github]](https://github.com/nlohmann/json)

## How to build?
* Install Visual Studio
* Open the "GestureControl.sln" solution file in Visual Studio
* Choose appropriate configuration (Debug or Release)
* Click "Build"->"Build solution" or press F7 to build solution
