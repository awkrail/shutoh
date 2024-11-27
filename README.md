# Scene detector
This repo is a fast scene detector implemented in C++.

# Build
Install dependency libraries: openCV.
```
# Linux
apt install libopencv-dev
# Mac OS
brew install opencv
# Windows
vcpkg install opencv
```
Build with cmake as:
```
mkdir build
cd build
cmake ..
cmake --build .
```

# Run
```
./scene_detector --input_path ../video/input.mp4
```