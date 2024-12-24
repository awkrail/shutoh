# Shutoh
Shutoh (手刀 in Japanese) is a fast scene detector implemented in C++20.

# Build
Install dependency libraries: openCV.
```
# Linux
apt install libopencv-dev
# Mac OS
brew install opencv
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
./build/shutoh -i video/input.mp4 -c list-scenes
```

# Test
```
cd build
ctest
```