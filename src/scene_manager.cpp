#include "opencv2/opencv.hpp"
#include "video_stream.hpp"
#include "scene_manager.hpp"
#include "frame_timecode.hpp"
#include "blocking_queue.hpp"

#include <stdexcept>
#include <thread>
#include <queue>
#include <functional>

// debug
#include <chrono>

namespace scene_manager {

const int32_t DEFAULT_MIN_WIDTH = 256;
const int32_t MAX_FRAME_QUEUE_LENGTH = 4;

SceneManager::SceneManager() {}

void SceneManager::detect_scenes(video_stream::VideoStream& video) {
    frame_timecode::FrameTimeCode base_timecode_ = video.base_timecode();
    int32_t total_frames = video.duration().get_frame_num();
    int32_t downscale_factor = compute_downscale_factor(video.width());

    BlockingQueue<VideoFrame> frame_queue(MAX_FRAME_QUEUE_LENGTH);
    std::thread thread(&SceneManager::_decode_thread,
                       this,
                       std::ref(video),
                       downscale_factor,
                       std::ref(frame_queue));
    
    int counter = 10;
    while (counter > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        VideoFrame video_frame = frame_queue.get();
    }
    thread.join();
}

void SceneManager::_decode_thread(video_stream::VideoStream& video,
                                  int32_t downscale_factor, 
                                  BlockingQueue<VideoFrame>& frame_queue) {
    
    const int32_t width = video.width();
    const int32_t height = video.height();

    while (true) {
        cv::Mat frame;

        if(!video.get_cap().read(frame)) {
            break;
        }

        /**
        if (downscale_factor > 1) {
            cv::resize(frame, frame, cv::Size(),
                       std::round(width / downscale_factor),
                       std::round(height / downscale_factor));
        }
        */
        VideoFrame video_frame;
        video_frame.frame = frame;
        video_frame.position = video.position();
        frame_queue.push(video_frame);
        std::cout << "Size: " << frame_queue.size() << " Position: " << video_frame.position.get_frame_num() << std::endl;
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