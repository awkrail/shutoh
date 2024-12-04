#include "opencv2/opencv.hpp"
#include "video_stream.hpp"
#include "scene_manager.hpp"
#include "frame_timecode.hpp"

#include <stdexcept>
#include <thread>
#include <queue>
#include <functional>

namespace scene_manager {

const int32_t DEFAULT_MIN_WIDTH = 256;
const int32_t MAX_FRAME_QUEUE_LENGTH = 4;

SceneManager::SceneManager() {}

void SceneManager::detect_scenes(VideoStream& video) {
    frame_timecode::FrameTimeCode base_timecode_ = video.base_timecode();
    int32_t total_frames = video.duration().get_frame_num();
    int32_t downscale_factor = compute_downscale_factor(video.width());

    std::queue<cv::Mat> frame_queue;
    std::thread thread(&SceneManager::_decode_thread,
                       this, 
                       std::ref(video),
                       downscale_factor,
                       std::ref(frame_queue));
    thread.join();

    while (!frame_queue.empty()) {
        std::cout << frame_queue.front() << " ";
        frame_queue.pop();
    }

}

void SceneManager::_decode_thread(VideoStream& video,
                                  int32_t downscale_factor, 
                                  std::queue<cv::Mat>& frame_queue) {
    
    const int32_t width = video.width();
    const int32_t height = video.height();

    while (true) {
        cv::Mat frame;

        if(!video.get_cap().read(frame)) {
            break;
        }

        if (downscale_factor > 1) {
            frame = cv::resize(frame, frame, cv::Size(),
                               std::round(width / downscale_factor),
                               std::round(height / downscale_factor));
        }
        


        std::cout << frame << std::endl;
        break;
    }
    frame_queue.push(3);
    frame_queue.push(4);
    frame_queue.push(5);
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