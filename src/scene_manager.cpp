#include "scene_manager.hpp"
#include "frame_timecode.hpp"
#include "video_frame.hpp"
#include "video_stream.hpp"
#include "blocking_queue.hpp"
#include "error.hpp"

#include <thread>

constexpr int32_t DEFAULT_MIN_WIDTH = 256;
constexpr int32_t MAX_FRAME_QUEUE_LENGTH = 100;

SceneManager::SceneManager(ContentDetector& detector) : detector_{detector} {}

void SceneManager::detect_scenes(VideoStream& video) {
    start_ = video.get_start();
    end_ = video.get_end();
    framerate_ = video.get_framerate();

    const int32_t start_frame_num = start_.value().get_frame_num();
    video.seek(start_frame_num);

    BlockingQueue<VideoFrame> frame_queue(MAX_FRAME_QUEUE_LENGTH);
    const int32_t downscale_factor = compute_downscale_factor(video.width());
    std::thread thread(&SceneManager::_decode_thread,
                       this,
                       std::ref(video),
                       downscale_factor,
                       std::ref(frame_queue));

    while (true) {
        VideoFrame next_frame = frame_queue.get();        
        _process_frame(next_frame);

        if (next_frame.is_end_frame)
            break;
    }
    thread.join();
}

WithError<std::vector<FrameTimeCodePair>> SceneManager::get_scene_list() const {
    if (!start_.has_value() || !end_.has_value()) {
        std::string error_msg = "Run detect_scenes() before get_scene_list().";
        return WithError<std::vector<FrameTimeCodePair>> { std::nullopt, Error(ErrorCode::FunctionIsNotCalled, error_msg) };
    }

    const FrameTimeCode start_pos = start_.value();
    const FrameTimeCode last_pos = end_.value();
    std::vector<FrameTimeCode> timecode_cut_list = _get_cutting_list();
    std::vector<FrameTimeCodePair> scenes;

    if (timecode_cut_list.size() == 0) {
        FrameTimeCodePair scene{ start_pos, last_pos }; 
        scenes.push_back(scene);
        return WithError<std::vector<FrameTimeCodePair>> { scenes, Error(ErrorCode::Success, "") };
    }

    FrameTimeCode last_cut = start_pos;
    for (auto& cut : timecode_cut_list) {
        FrameTimeCodePair scene{ last_cut, cut };
        scenes.push_back(scene);
        last_cut = cut;
    }

    FrameTimeCodePair scene{ last_cut, last_pos };
    scenes.push_back(scene);    
    return WithError<std::vector<FrameTimeCodePair>> { scenes, Error(ErrorCode::Success, "") };
}

std::vector<FrameTimeCode> SceneManager::_get_cutting_list() const {
    std::vector<FrameTimeCode> timecode_list;
    for(auto& cut : cutting_list_) {
        const FrameTimeCode timecode = frame_timecode::from_frame_nums(cut, framerate_).value();
        timecode_list.push_back(timecode);
    }
    return timecode_list;
}

void SceneManager::_process_frame(VideoFrame& next_frame) {
    std::optional<int32_t> cuts = detector_.process_frame(next_frame);
    if (cuts.has_value())
        cutting_list_.push_back(cuts.value());
}

void SceneManager::_decode_thread(VideoStream& video,
                                  int32_t downscale_factor, 
                                  BlockingQueue<VideoFrame>& frame_queue) {

    while (true) {
        cv::Mat frame;

        if(!video.get_cap().read(frame))
            continue;

        if (downscale_factor > 1) {
            int32_t new_width = static_cast<int32_t>(round(frame.cols / downscale_factor));
            int32_t new_height = static_cast<int32_t>(round(frame.rows / downscale_factor));
            cv::Size new_size(new_width, new_height);
            cv::resize(frame, frame, new_size, 0, 0, cv::INTER_LINEAR);
        }

        const int32_t num_pixels = frame.rows * frame.cols;
        VideoFrame video_frame {frame, video.position(), video.is_end_frame(), num_pixels};
        frame_queue.push(video_frame);

        if (video.is_end_frame())
            break;
    }
}

int32_t compute_downscale_factor(const int32_t frame_width) {
    if (frame_width < DEFAULT_MIN_WIDTH)
        return 1;

    return static_cast<int32_t>(frame_width / DEFAULT_MIN_WIDTH);
}