#ifndef FRAME_TIMECODE_H
#define FRAME_TIMECODE_H

#include <memory>
#include <string>

double MAX_FPS_DELTA = 1.0 / 100000;
double _SECONDS_PER_MINUTE = 60.0;
double _SECONDS_PER_HOUR = 60.0 * _SECONDS_PER_MINUTE;
double _MINUTES_PER_HOUR = 60.0;

struct HourMinSec {
    long long int hrs;
    int mins;
    int secs;
};

class FrameTimeCode {
    public:
        FrameTimeCode(const FrameTimeCode& timecode);
        FrameTimeCode(const std::string& timecode_str, const double fps);
        FrameTimeCode(const double timecode_num, const double fps);
        FrameTimeCode(const int timecode_num, const double fps);

    private:
        const long long int _parse_timecode_string(const std::string& timecode_str);
        const long long int _parse_timecode_number(const int timecode_num);
        const long long int _parse_timecode_number(const double timecode_num);

        const HourMinSec _parse_hrs_mins_secs_to_second(const std::string& timecode_str) const;

        double framerate_;
        long long int frame_num_;

};

#endif