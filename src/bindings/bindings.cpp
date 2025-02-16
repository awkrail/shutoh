#include <shutoh/video_stream.hpp>
#include <shutoh/scene_manager.hpp>
#include <shutoh/detector/content_detector.hpp>
#include <pybind11/pybind11.h>

void _detect(const std::string& input_path) {
    WithError<VideoStream> opt_video = VideoStream::initialize_video_stream(input_path);
    if (opt_video.has_error())
        throw std::runtime_error(opt_video.error.get_error_msg());
    VideoStream video = opt_video.value();

    auto detector = ContentDetector::initialize_detector();
    SceneManager scene_manager = SceneManager(std::move(detector));
    scene_manager.detect_scenes(video);
    std::vector<FrameTimeCodePair> scene_list = scene_manager.get_scene_list().value();
    for (auto& scene : scene_list) {
        const FrameTimeCode start = std::get<0>(scene);
        const FrameTimeCode end = std::get<1>(scene);
        std::cout << "Start Time: " << start.to_string() << " Frame: " << start.get_frame_num()
        << " / End Time: " << end.to_string() << " Frame: " << end.get_frame_num() << std::endl; 
    }
}

PYBIND11_MODULE(libshutoh, m) {
    m.def("detect", &_detect, "A function that detects shots from a video");
}