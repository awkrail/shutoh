#include <iostream>
#include <argparse/argparse.hpp>
#include <tuple>
#include "video_stream.hpp"
#include "frame_timecode.hpp"
#include "scene_manager.hpp"
#include "content_detector.hpp"
#include "command_runner.hpp"

using FrameTimeCodePair = std::tuple<FrameTimeCode, FrameTimeCode>;

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
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    std::string input_path = program.get<std::string>("--input");
    std::string command = program.get<std::string>("--command");
    std::string output_path = program.get<std::string>("--output");

    WithError<VideoStream> opt_video = VideoStream::initialize_video_stream(input_path);
    if (opt_video.has_error()) {
        std::cerr << opt_video.error.get_error_msg() << std::endl;
        return 1;
    }

    VideoStream video = opt_video.value();
    ContentDetector detector = ContentDetector();
    SceneManager scene_manager = SceneManager(detector);
    scene_manager.detect_scenes(video);
    
    WithError<std::vector<FrameTimeCodePair>> scene_list = scene_manager.get_scene_list();
    if (scene_list.has_error()) {
        std::cerr << scene_list.error.get_error_msg() << std::endl;
        return 1;
    }

    CommandRunner command_runner = CommandRunner(input_path, command, output_path, scene_list);
    command_runner.execute();
    return 0;
}