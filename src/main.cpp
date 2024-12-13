#include <iostream>
#include <argparse/argparse.hpp>
#include <tuple>
#include "video_stream.hpp"
#include "scene_manager.hpp"
#include "content_detector.hpp"
#include "command_runner.hpp"

using FrameTimeCodePair = std::tuple<frame_timecode::FrameTimeCode, frame_timecode::FrameTimeCode>;

video_stream::VideoStream open_video(const std::string& input_path) {
  video_stream::VideoStream video_stream = video_stream::VideoStream(input_path);
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

    video_stream::VideoStream video = open_video(input_path);
    content_detector::ContentDetector detector = content_detector::ContentDetector();
    scene_manager::SceneManager scene_manager = scene_manager::SceneManager(detector);
    int32_t ret = scene_manager.detect_scenes(video);
    if (ret) {
        std::cerr << "Error: scene detection failed." << std::endl;
    }
    
    std::vector<FrameTimeCodePair> scene_list = scene_manager.get_scene_list();
    if (scene_list.size() == 0) {
      std::cerr << "Error: scene list is empty." << std::endl;
    }

    command_runner::CommandRunner command_runner = command_runner::CommandRunner(input_path, command, 
                                                                                 output_path, scene_list);
    command_runner.execute();

    //for(auto& scene : scene_list) {
    //  std::cout << std::get<0>(scene).to_string() << " " << std::get<1>(scene).to_string() << std::endl;
    //}

    return 0;
}