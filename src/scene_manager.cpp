#include "video_stream.hpp"
#include "scene_manager.hpp"
#include "frame_timecode.hpp"

SceneManager::SceneManager() {}

void SceneManager::detect_scenes(VideoStream& video) {
    FrameTimeCode base_timecode_ = video.base_timecode();
    // downscale_factor = compute_downscale_factor(video.frame_size[0])
}