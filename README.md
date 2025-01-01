# Shutoh
Shutoh (手刀 in Japanese) is a fast and efficient scene detection tool implemented in C++20.
Inspired by [PySceneDetect](https://github.com/Breakthrough/PySceneDetect), Shutoh aims to provide a powerful and flexible alternative with enhanced performance. The key features of Shutoh is threefold:
- **Fast**: Shutoh leverages the speed of C++ for faster processing compared to PySceneDetect.
- **Compatibility**: Designed to produce outputs comparable to PySceneDetect as much as possible.
- **Flexible**: Supports both rule-based and machine-learning-based approaches.

**Note:** This project is work in progress and the current codebase may be changed signficantly.

## Build
Install dependency libraries: FFmpeg and openCV. If you are an Ubuntu user, run:
```
apt install libopencv-dev ffmpeg
```
Then, build `shutoh` with `cmake` as:
```
cmake -S . -B build
cmake --build build
```
For develoers, run test using `ctest` as:
```
cd build
ctest
```

## Quick Start (Command Line)
I focus on three commands (`split-video`, `list-scenes`, and `save-images`) and remove several options for simplicity.
Split an input video into cuts:
```
./build/shutoh -i video/input.mp4 -c split-video
```
Save scenes as csv file:
```
./build/shutoh -i video/input.mp4 -c list-scenes
```

## Limitations
- Currently, Shutoh supports videos up to **10 hours** at **120 fps**.

## Contribution
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## LICENSE
MIT License

## Contact
Taichi Nishimura ([taichitary@gmail.com](taichitary@gmail.com))  
Copyright (C) 2024-2025 Taichi Nishimura.
All rights reserved.
