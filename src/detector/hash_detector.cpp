#include "hash_detector.hpp"

HashDetector::HashDetector(const float threshold, const int32_t min_scene_len,
                           const int32_t size, const int32_t lowpass) : threshold_{threshold}, min_scene_len_{min_scene_len}, 
                           size_{size}, size_sq_{size * size}, factor_{lowpass} {}

std::optional<int32_t> HashDetector::process_frame(VideoFrame& next_frame) {
}