#include "shutoh/video_stream.hpp"
#include "shutoh/scene_manager.hpp"
#include "shutoh/frame_timecode_pair.hpp"
#include "shutoh/detector/base_detector.hpp"
#include "shutoh/detector/content_detector.hpp"
#include "shutoh/detector/hash_detector.hpp"
#include "shutoh/detector/histogram_detector.hpp"
#include "shutoh/detector/threshold_detector.hpp"
#include "shutoh/detector/adaptive_detector.hpp"

#include <catch2/catch_test_macros.hpp>

enum class DetectorType {
    CONTENT,
    HASH,
    HISTOGRAM,
    ADAPTIVE,
    THRESHOLD,
    OTHER,
};

std::unique_ptr<BaseDetector> _select_default_detector(const DetectorType detector_type) {
    switch (detector_type) {
        case DetectorType::CONTENT:
            return std::make_unique<ContentDetector>();
        case DetectorType::HASH:
            return std::make_unique<HashDetector>();
        case DetectorType::HISTOGRAM:
            return std::make_unique<HistogramDetector>();
        case DetectorType::THRESHOLD:
            return std::make_unique<ThresholdDetector>();
        case DetectorType::ADAPTIVE:
            return std::make_unique<AdaptiveDetector>();
        default: 
            return std::make_unique<ContentDetector>();
    }
}

std::vector<FrameTimeCodePair> _get_scenes(DetectorType detector_type) {
    const std::string input_path = "../../video/input.mp4";
    VideoStream video = VideoStream::initialize_video_stream(input_path).value();
    auto detector = _select_default_detector(detector_type);
    SceneManager scene_manager = SceneManager(std::move(detector));
    scene_manager.detect_scenes(video);
    std::vector<FrameTimeCodePair> scene_list = scene_manager.get_scene_list().value();
    return scene_list;
}

void test_frame_index(const std::vector<FrameTimeCodePair> scene_list,
                      const std::vector<int32_t> expected_inds) {
    for (int i = 0; i < scene_list.size(); i++) {
        const int32_t pred_frame_ind = std::get<0>(scene_list[i]).get_frame_num();
        REQUIRE(pred_frame_ind == expected_inds[i]);
    }
}

TEST_CASE("SceneManager - content detector", "[SceneManager scene_detect]") {
    const DetectorType detector_type = DetectorType::CONTENT;
    std::vector<FrameTimeCodePair> scene_list = _get_scenes(detector_type);
    /* content detector's frame indices */
    std::vector<int32_t> expected_inds {
        0, 64, 143, 194, 254, 314, 419, 628, 712, 809, 880, 965, 1125,
        1211, 1250, 1330, 1391, 1449, 1529, 1611, 1704, 1804, 2000, 2207,
        2541, 2872, 2913, 2992, 3072, 3151, 3212, 3271, 3352, 3453, 3618,
        4122, 4257 };
    test_frame_index(scene_list, expected_inds);
}

TEST_CASE("SceneManager - hash detector", "[SceneManager scene_detect]") {
    const DetectorType detector_type = DetectorType::HASH;
    std::vector<FrameTimeCodePair> scene_list = _get_scenes(detector_type);
    std::vector<int32_t> expected_inds {
        0, 64, 127, 143, 194, 254, 314, 419, 712, 809, 880, 965, 1125, 1211,
        1250, 1330, 1391, 1449, 1529, 1611, 1704, 1804, 2000, 2207, 2541, 2872,
        2913, 2992, 3072, 3151, 3212, 3271, 3352, 3453, 3618, 4122, 4257 };
    test_frame_index(scene_list, expected_inds);
}

TEST_CASE("SceneManager - hist detector", "[SceneManager scene_detect]") {
    const DetectorType detector_type = DetectorType::HISTOGRAM;
    std::vector<FrameTimeCodePair> scene_list = _get_scenes(detector_type);
    std::vector<int32_t> expected_inds {
        0, 64, 124, 143, 194, 254, 297, 312, 377, 409, 628, 712, 809, 880, 965,
        1125, 1211, 1250, 1330, 1391, 1449, 1529, 1611, 1704, 1804, 2000, 2207,
        2354, 2541, 2872, 2913, 2992, 3072, 3151, 3212, 3271, 3352, 3453, 3618,
        4122, 4257, 4316, 4397 };
    test_frame_index(scene_list, expected_inds);
}

TEST_CASE("SceneManager - threshold detector", "[SceneManager scene_detect]") {
    const DetectorType detector_type = DetectorType::THRESHOLD;
    std::vector<FrameTimeCodePair> scene_list = _get_scenes(detector_type);
    std::vector<int32_t> expected_inds { 0 };
    test_frame_index(scene_list, expected_inds);
}

TEST_CASE("SceneManager - adaptive detector", "[SceneManager scene_detect]") {
    const DetectorType detector_type = DetectorType::ADAPTIVE;
    std::vector<FrameTimeCodePair> scene_list = _get_scenes(detector_type);
    std::vector<int32_t> expected_inds {
        0, 64, 143, 194, 254, 314, 419, 628, 712, 809, 880, 965, 1125, 1211, 
        1250, 1330, 1391, 1449, 1529, 1611, 1704, 1804, 2000, 2207, 2541, 2872, 
        2913, 2992, 3072, 3151, 3212, 3271, 3352, 3453, 3618, 4122, 4257, 4316, 4397 };
    test_frame_index(scene_list, expected_inds);
}