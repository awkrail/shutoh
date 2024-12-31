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
    const std::optional<CommonConfig> opt_config = parse_args(argc, argv);
    if (!opt_config) {
        return 1;
    }
    const CommonConfig& config = *opt_config;

    std::filesystem::path input_path = config.input_path;
    std::filesystem::path output_path = config.output_dir;
    std::string command = config.command;

    WithError<VideoStream> opt_video = VideoStream::initialize_video_stream(input_path);
    if (opt_video.has_error()) {
        opt_video.error.show_error_msg();
        return 1;
    }

    VideoStream video = opt_video.value();
    ContentDetector detector = ContentDetector();
    SceneManager scene_manager = SceneManager(detector);
    scene_manager.detect_scenes(video);
    
    WithError<std::vector<FrameTimeCodePair>> opt_scene_list = scene_manager.get_scene_list();
    if (opt_scene_list.has_error()) {
        opt_scene_list.error.show_error_msg();
        return 1;
    }

    std::vector<FrameTimeCodePair> scene_list = opt_scene_list.value();
    WithError<CommandRunner> opt_command_runner = CommandRunner::initialize_command_runner(input_path, command, output_path, scene_list);
    if (opt_command_runner.has_error()) {
        opt_command_runner.error.show_error_msg();
        return 1;
    }

    CommandRunner command_runner = opt_command_runner.value();
    WithError<void> err = command_runner.execute(video);
    if (err.has_error()) {
        err.error.show_error_msg();
        return 1;
    }

    return 0;
}