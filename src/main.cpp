#include "frame_timecode_pair.hpp"
#include "video_stream.hpp"
#include "frame_timecode.hpp"
#include "scene_manager.hpp"
#include "content_detector.hpp"
#include "command_runner.hpp"

#include <iostream>
#include <argparse/argparse.hpp>
#include <filesystem>

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("shutoh");

    program.add_argument("-i", "--input")
        .help("Input video file.")
        .required();
    
    program.add_argument("-c", "--command")
        .help("Command name. Choose one from [list-scenes, split-videos, save-images]")
        .required();

    program.add_argument("-o", "--output")
        .default_value(std::string("."))
        .help("Output directory for created files. If unset, working directory will be used.");

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return 1;
    }

    std::string input_path_str = program.get<std::string>("--input");
    std::string command = program.get<std::string>("--command");
    std::string output_path_str = program.get<std::string>("--output");

    std::filesystem::path input_path(input_path_str);
    std::filesystem::path output_path(output_path_str);

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