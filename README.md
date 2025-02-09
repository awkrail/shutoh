# Shutoh
Shutoh (手刀, meaning karate chop in Japanese) is a fast and efficient scene detection tool implemented in C++20.
Inspired by [PySceneDetect](https://github.com/Breakthrough/PySceneDetect), Shutoh aims to provide a powerful and flexible alternative with enhanced performance. The key features of Shutoh is threefold:
- **Fast**: Shutoh leverages the speed of C++ for faster processing compared to PySceneDetect.
- **Compatibility**: Designed to produce outputs comparable to PySceneDetect as much as possible.
- **Flexible**: Supports both rule-based and machine-learning-based approaches.

**Note:** This project is work in progress and the current codebase may be changed signficantly.

# Performance
[WIP]: Evaluated results

# Installation
[WIP]: Installation

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
Run test using `ctest` as:
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

## API
### Python
WIP

### C++
The simpletest code is as follow:
```cpp
#include "shutoh/video_stream.hpp"
#include "shutoh/frame_timecode.hpp"
#include "shutoh/scene_manager.hpp"
#include "shutoh/frame_timecode_pair.hpp"
#include "shutoh/detector/content_detector.hpp"

int main() {
    VideoStream video = VideoStream::initialize_video_stream("../video/input.mp4").value();
    auto detector = ContentDetector::initialize_detector();
    SceneManager scene_manager = SceneManager(std::move(detector));
    scene_manager.detect_scenes(video);
    std::vector<FrameTimeCodePair> scene_list = scene_manager.get_scene_list().value();

    for (auto& scene : scene_list) {
        const FrameTimeCode start = std::get<0>(scene);
        const FrameTimeCode end = std::get<1>(scene);
        std::cout << "Start Time: " << start.to_string() << " Frame: " << start.get_frame_num()
        << " / End Time: " << end.to_string() << " Frame: " << end.get_frame_num() << std::endl; 
    }
}
```
To compile the code, run the following g++ command (replace `-I` and `-L` with your directory):
```
g++ -std=c++20 -I/path/to/shutoh/include -I/usr/include/opencv4 main.cc -L/path/to/shutoh/build -lopencv_core -lopencv_videoio -Wl,-rpath,/path/to/build -lshutoh_lib
```

## Contribution
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## LICENSE
MIT License

## Contact
Taichi Nishimura ([taichitary@gmail.com](taichitary@gmail.com))  
Copyright (C) 2024-2025 Taichi Nishimura.
All rights reserved.