#ifndef FRAME_TIMECODE_H
#define FRAME_TIMECODE_H

#include <memory>
#include <string>
#include <cstdint>

extern const double MAX_FPS_DELTA;
extern const double _SECONDS_PER_MINUTE;
extern const double _SECONDS_PER_HOUR;
extern const double _MINUTES_PER_HOUR;

struct HourMinSec {
    int32_t hrs;
    int8_t mins;
    int8_t secs;
};

class FrameTimeCode {
    public:
        FrameTimeCode(const FrameTimeCode& timecode);
        FrameTimeCode(const std::string& timecode_str, const double fps);
        FrameTimeCode(const double timecode_num, const double fps);
        FrameTimeCode(const int32_t timecode_num, const double fps);

    private:
        const int32_t _parse_timecode_string(const std::string& timecode_str);
        const int32_t _parse_timecode_number(const int32_t timecode_num);
        const int32_t _parse_timecode_number(const double timecode_num);

        const HourMinSec _parse_hrs_mins_secs_to_second(const std::string& timecode_str) const;
        const int32_t _seconds_to_frames(const double seconds) const;

        double framerate_;
        int32_t frame_num_;
};

#endif