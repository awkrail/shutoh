#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "frame_timecode.hpp"
#include "video_stream.hpp"

class SceneManager {
    public:
        SceneManager();
        void detect_scenes(VideoStream& video);

    private:
        // FrameTimeCode start_pos_;
        // FrameTimeCode end_pos_;

};

#endif