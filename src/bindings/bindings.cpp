#include <shutoh/video_stream.hpp>
#include <shutoh/scene_manager.hpp>
#include <shutoh/detector/base_detector.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

void bind_base_detector(pybind11::module_ &m);
void bind_content_detector(pybind11::module_ &m);
void bind_adaptive_detector(pybind11::module_ &m);
void bind_hash_detector(pybind11::module_ &m);
void bind_histogram_detector(pybind11::module_ &m);
void bind_threshold_detector(pybind11::module_ &m);

std::vector<std::tuple<pybind11::object, pybind11::object>> create_frame_timecode_list(const std::vector<FrameTimeCodePair>& scene_list) {
    /* Shutoh: FrameTimeCode, PySceneDetect: FrameTimecode */
    /* TODO: Remove pyscenedetect.frame_timecode dependency */
    std::vector<std::tuple<pybind11::object, pybind11::object>> out;

    pybind11::module_ py_frame_timecode = pybind11::module_::import("scenedetect.frame_timecode");
    pybind11::object FrameTimecode = py_frame_timecode.attr("FrameTimecode");

    for (auto& scene : scene_list) {
        const FrameTimeCode start = std::get<0>(scene);
        const FrameTimeCode end = std::get<1>(scene);

        const pybind11::object start_py = FrameTimecode(start.get_frame_num(), start.get_framerate());
        const pybind11::object end_py = FrameTimecode(end.get_frame_num(), end.get_framerate());
        
        auto scene_py = std::make_tuple(start_py, end_py);
        out.push_back(scene_py);
    }

    return out;
}

auto _detect(const std::string& input_path, std::shared_ptr<BaseDetector> detector) {
    WithError<VideoStream> opt_video = VideoStream::initialize_video_stream(input_path);
    if (opt_video.has_error())
        throw std::runtime_error(opt_video.error.get_error_msg());
    VideoStream video = opt_video.value();

    SceneManager scene_manager = SceneManager(detector);
    scene_manager.detect_scenes(video);
    std::vector<FrameTimeCodePair> scene_list = scene_manager.get_scene_list().value();
    auto scene_list_py = create_frame_timecode_list(scene_list);
    return scene_list_py;
}

PYBIND11_MODULE(libshutoh, m) {
    bind_base_detector(m);
    bind_content_detector(m);
    bind_adaptive_detector(m);
    bind_hash_detector(m);
    bind_histogram_detector(m);
    bind_threshold_detector(m);
    m.def("detect", &_detect, "A function that detects shots from a video");
}