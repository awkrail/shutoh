#ifndef FRAME_TIMECODE_H
#define FRAME_TIMECODE_H

#include <memory>
#include <string>
#include <cstdint>

namespace frame_timecode {

extern const float MAX_FPS_DELTA;
extern const float _SECONDS_PER_MINUTE;
extern const float _SECONDS_PER_HOUR;
extern const float _MINUTES_PER_HOUR;

struct HourMinSec {
    int32_t hrs;
    int32_t mins;
    float secs;
};

class FrameTimeCode {
    public:
        FrameTimeCode(const FrameTimeCode& timecode);
        FrameTimeCode(const int32_t frame_num, const float fps);

        float get_framerate() const { return framerate_; }
        int32_t get_frame_num() const { return frame_num_; }

        void set_framerate(const float framerate) { framerate_ = framerate; }
        void set_frame_num(const int32_t frame_num) { frame_num_ = frame_num; }

        int32_t parse_timecode_string(const std::string& timecode_str) const;
        int32_t parse_timecode_number(const int32_t seconds) const;
        int32_t parse_timecode_number(const float seconds) const;

        bool operator==(const FrameTimeCode& other) const;
        bool operator!=(const FrameTimeCode& other) const;
        bool operator<(const FrameTimeCode& other) const;
        bool operator>(const FrameTimeCode& other) const;
        bool operator<=(const FrameTimeCode& other) const;
        bool operator>=(const FrameTimeCode& other) const;

        const FrameTimeCode operator+(const FrameTimeCode& other) const;
        const FrameTimeCode operator-(const FrameTimeCode& other) const;

    private:
        const HourMinSec _parse_hrs_mins_secs_to_second(const std::string& timecode_str) const;
        int32_t _seconds_to_frames(const float seconds) const;

        float framerate_;
        int32_t frame_num_;
};

const FrameTimeCode from_timecode_string(const std::string& timecode_str, const float fps);
const FrameTimeCode from_frame_nums(const int32_t frame_num, const float fps);
const FrameTimeCode from_seconds(const float seconds, const float fps);
const FrameTimeCode from_seconds(const int32_t seconds, const float fps);

}

#endif