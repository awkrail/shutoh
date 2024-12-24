#include "opencv2/opencv.hpp"
#include "scene_manager.hpp"
#include "frame_timecode.hpp"
#include "video_frame.hpp"
#include "video_stream.hpp"
#include "blocking_queue.hpp"
#include "content_detector.hpp"

#include <stdexcept>
#include <thread>
#include <queue>
#include <functional>
#include <optional>
#include <tuple>

using FrameTimeCodePair = std::tuple<FrameTimeCode, FrameTimeCode>;

const int32_t DEFAULT_MIN_WIDTH = 256;
const int32_t MAX_FRAME_QUEUE_LENGTH = 4;

SceneManager::SceneManager(ContentDetector& detector) : detector_{detector} {}

void SceneManager::detect_scenes(VideoStream& video) {
    base_timecode_ = video.base_timecode();
    framerate_ = video.get_framerate();
    int32_t total_frames = video.duration().get_frame_num();
    int32_t downscale_factor = compute_downscale_factor(video.width());

    BlockingQueue<VideoFrame> frame_queue(MAX_FRAME_QUEUE_LENGTH);
    std::thread thread(&SceneManager::_decode_thread,
                       this,
                       std::ref(video),
                       downscale_factor,
                       std::ref(frame_queue));
    
    bool flg = false;

    while (true) {
        VideoFrame next_frame = frame_queue.get();
        if (next_frame.is_end_frame) {
            break;
        }
        _process_frame(next_frame);
    }
    thread.join();
    if(!last_pos_.has_value()) {
        last_pos_ = video.position();
    }
}

std::vector<FrameTimeCodePair> SceneManager::get_scene_list() const {
    if (!base_timecode_.has_value()) {
        std::runtime_error("Base Timecode is not set. Run detect_scenes() before running get_scene_list().");
    }
    std::vector<FrameTimeCode> timecode_cut_list = _get_cutting_list();

    std::vector<FrameTimeCodePair> scenes;
    if (timecode_cut_list.size() == 0) {
        FrameTimeCodePair scene{ start_pos_.value(), last_pos_.value() }; 
        scenes.push_back(scene);
    }

    FrameTimeCode last_cut = start_pos_.value();
    for (auto& cut : timecode_cut_list) {
        FrameTimeCodePair scene{ last_cut, cut };
        scenes.push_back(scene);
        last_cut = cut;
    }

    FrameTimeCodePair scene{ last_cut, last_pos_.value() };
    scenes.push_back(scene);
    
    return scenes;
}

std::vector<FrameTimeCode> SceneManager::_get_cutting_list() const {
    std::vector<FrameTimeCode> timecode_list;
    for(auto& cut : cutting_list_) {
        const FrameTimeCode timecode = base_timecode_.value() + frame_timecode::from_frame_nums(cut, framerate_).value();
        timecode_list.push_back(timecode);
    }
    return timecode_list;
}

void SceneManager::_process_frame(VideoFrame& next_frame) {
    std::optional<int32_t> cuts = detector_.process_frame(next_frame);
    if (cuts.has_value()) {
        cutting_list_.push_back(cuts.value());
    }
}

void SceneManager::_decode_thread(VideoStream& video,
                                  int32_t downscale_factor, 
                                  BlockingQueue<VideoFrame>& frame_queue) {
    
    const int32_t width = video.width();
    const int32_t height = video.height();

    while (true) {
        cv::Mat frame;

        if(!video.get_cap().read(frame)) {
            break;
        }

        if (downscale_factor > 1) {
            int32_t new_width = static_cast<int32_t>(round(frame.cols / downscale_factor));
            int32_t new_height = static_cast<int32_t>(round(frame.rows / downscale_factor));
            cv::Size new_size(new_width, new_height);
            cv::resize(frame, frame, new_size, 0, 0, cv::INTER_LINEAR);
        }
        
        if(!start_pos_.has_value()) {
            start_pos_ = video.position();
        }

        const int32_t num_pixels = frame.rows * frame.cols;
        VideoFrame video_frame {frame, video.position(), video.is_end_frame(), num_pixels};
        frame_queue.push(video_frame);
    }
}

const int32_t compute_downscale_factor(const int32_t frame_width) {
    if (frame_width < 0) {
        throw std::invalid_argument("frame_width should be larger than 0.");
    }
    if (frame_width < DEFAULT_MIN_WIDTH) {
        return 1;
    }
    return static_cast<int32_t>(frame_width / DEFAULT_MIN_WIDTH);
}