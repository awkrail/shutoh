#include <iostream>
#include <argparse/argparse.hpp>
#include <tuple>
#include "video_stream.hpp"
#include "frame_timecode.hpp"
// #include "scene_manager.hpp"
// #include "content_detector.hpp"
// #include "command_runner.hpp"

using FrameTimeCodePair = std::tuple<frame_timecode::FrameTimeCode, frame_timecode::FrameTimeCode>;

WithError<video_stream::VideoStream> open_video(const std::string& input_path) {
    WithError<video_stream::VideoStream> video_stream = video_stream::initialize_video_stream(input_path);
    return video_stream;
}

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

    WithError<video_stream::VideoStream> video = open_video(input_path);
    if (video.error.has_error()) {
        std::cerr << video.error.get_error_msg() << std::endl;
        return 1;
    }

    /*
    content_detector::ContentDetector detector = content_detector::ContentDetector();
    scene_manager::SceneManager scene_manager = scene_manager::SceneManager(detector);
    scene_manager.detect_scenes(video);
    std::vector<FrameTimeCodePair> scene_list = scene_manager.get_scene_list();

    command_runner::CommandRunner command_runner = command_runner::CommandRunner(input_path, command, 
                                                                                 output_path, scene_list);
    command_runner.execute();
    */
    return 0;
}