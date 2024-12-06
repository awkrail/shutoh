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

namespace scene_manager {

const int32_t DEFAULT_MIN_WIDTH = 256;
const int32_t MAX_FRAME_QUEUE_LENGTH = 4;

SceneManager::SceneManager(content_detector::ContentDetector& detector) : detector_(detector) {}

void SceneManager::detect_scenes(video_stream::VideoStream& video) {
    frame_timecode::FrameTimeCode base_timecode_ = video.base_timecode();
    int32_t total_frames = video.duration().get_frame_num();
    int32_t downscale_factor = compute_downscale_factor(video.width());

    BlockingQueue<video_frame::VideoFrame> frame_queue(MAX_FRAME_QUEUE_LENGTH);
    std::thread thread(&SceneManager::_decode_thread,
                       this,
                       std::ref(video),
                       downscale_factor,
                       std::ref(frame_queue));
    
    while (true) {
        video_frame::VideoFrame next_frame = frame_queue.get();
        if (next_frame.is_end_frame) {
            break;
        }
        _process_frame(next_frame);
    }
    thread.join();
}

void SceneManager::_process_frame(video_frame::VideoFrame& next_frame) {
    std::vector<frame_timecode::FrameTimeCode> cuts = detector_.process_frame(next_frame);
    if (cuts.size() != 0) {
        cut_list_.push_back(cuts);
    }
}

void SceneManager::_decode_thread(video_stream::VideoStream& video,
                                  int32_t downscale_factor, 
                                  BlockingQueue<video_frame::VideoFrame>& frame_queue) {
    
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
        
        const int32_t num_pixels = video.width() * video.height();
        video_frame::VideoFrame video_frame {frame, video.position(), video.is_end_frame(), num_pixels};
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

}