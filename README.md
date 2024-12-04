# Shutoh
Shutoh (手刀 in Japanese) is a fast scene detector implemented in C++.

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
cmake -S . -B build
cmake --build build
```

# Limitations
The max hour of videos is 10h (120fps).

# Run
```
cd build
./shutoh --input_path ../video/input.mp4
```

# Test
```
cd build
ctest
```