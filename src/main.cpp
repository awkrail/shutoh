#include <iostream>
#include <argparse/argparse.hpp>
#include "video_stream.hpp"
#include "scene_manager.hpp"
#include "content_detector.hpp"

video_stream::VideoStream open_video(const std::string& input_path) {
  video_stream::VideoStream video_stream = video_stream::VideoStream(input_path);
  return video_stream;
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("scene_detector");

    program.add_argument("--input_path")
      .help("input video path")
      .required();

    try {
      program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
      std::cerr << err.what() << std::endl;
      std::cerr << program;
      return 1;
    }

    std::string input_path = program.get<std::string>("--input_path");
    video_stream::VideoStream video = open_video(input_path);
    content_detector::ContentDetector detector = content_detector::ContentDetector();
    scene_manager::SceneManager scene_manager = scene_manager::SceneManager(detector);
    scene_manager.detect_scenes(video);
    // auto scene_list = scene_manager.get_scene_list();
    // std::cout << scene_list << std::endl;

    return 0;
}