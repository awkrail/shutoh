# Shutoh

[![Build](https://github.com/awkrail/shutoh/actions/workflows/build.yml/badge.svg)](https://github.com/awkrail/shutoh/actions/workflows/build.yml)
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

Shutoh (手刀, meaning karate chop in Japanese) is a fast and efficient scene detection tool implemented in C++20.
Inspired by [PySceneDetect](https://github.com/Breakthrough/PySceneDetect), Shutoh aims to provide a powerful and flexible alternative with enhanced performance. The key features of Shutoh is threefold:
- **Fast**: Shutoh leverages the speed of C++ for faster processing compared to PySceneDetect.
- **Compatibility**: Designed to produce outputs comparable to PySceneDetect as much as possible.
- **Flexible**: Supports both rule-based and machine-learning-based approaches.

## Installation
Ensure that FFmpeg, OpenCV, and CMake are installed.
```
sudo apt install libopencv-dev ffmpeg cmake
```
To build `shutoh` with `cmake`, run:
```shell
cmake -S . -B build
cmake --build build
sudo cmake --install build
shutoh --help
```

## Quick Start (Command Line)
I focus on three main commands: `split-video`, `list-scenes`, and `save-images`. 
Save scene information (e.g., frame numbers and time) as csv file:
```
shutoh -i video/input.mp4 -c list-scenes
```
Split an input video into shots and save them as videos:
```
shutoh -i video/input.mp4 -c split-video
```
Save some frames from each shot:
```
shutoh -i video/input.mp4 -c list-scenes
```
See [documentation](docs/cli_documentation.md) for details.

## Contribution
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## LICENSE
MIT License

## Contact
Taichi Nishimura ([taichitary@gmail.com](taichitary@gmail.com))
Copyright (C) 2024-2025 Taichi Nishimura.
All rights reserved.
