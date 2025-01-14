#include "frame_timecode_pair.hpp"
#include "video_stream.hpp"
#include "frame_timecode.hpp"
#include "scene_manager.hpp"
#include "content_detector.hpp"
#include "command_runner.hpp"
#include "config.hpp"

#include <iostream>
#include <filesystem>

int main(int argc, char *argv[]) {
    const WithError<Config> opt_cfg = parse_args(argc, argv);
    if (opt_cfg.has_error()) {
        opt_cfg.error.show_error_msg();
        return 1;
    }
    Config cfg = opt_cfg.value();

    WithError<VideoStream> opt_video = VideoStream::initialize_video_stream(cfg.input_path, cfg.start, cfg.end);
    if (opt_video.has_error()) {
        opt_video.error.show_error_msg();
        return 1;
    }
    VideoStream video = opt_video.value();

    ContentDetector detector = ContentDetector(cfg.threshold, cfg.min_scene_len);
    SceneManager scene_manager = SceneManager(detector);
    scene_manager.detect_scenes(video);
    WithError<std::vector<FrameTimeCodePair>> opt_scene_list = scene_manager.get_scene_list();
    if (opt_scene_list.has_error()) {
        opt_scene_list.error.show_error_msg();
        return 1;
    }
    std::vector<FrameTimeCodePair> scene_list = opt_scene_list.value();
    
    CommandRunner command_runner = CommandRunner(cfg, scene_list);
    WithError<void> err = command_runner.execute(video);
    if (err.has_error()) {
        err.error.show_error_msg();
        return 1;
    }
    return 0;
}