#include "frame_timecode.hpp"
#include <stdexcept>
#include <cctype>
#include <string>
#include <vector>

FrameTimeCode::FrameTimeCode(const FrameTimeCode& timecode) : framerate_{0.0}, frame_num_{0} {
    framerate_ = timecode.framerate_;
    frame_num_ = timecode.frame_num_;
}

FrameTimeCode::FrameTimeCode(const std::string& timecode_str, double fps) : framerate_{0.0}, frame_num_{0} {
    if (fps < 0 || fps >= MAX_FPS_DELTA) {
        throw runtime_error("Framerate must be positive and greater than zero.");
    }
    framerate_ = fps;
    frame_num_ = _parse_timecode_string(timecode_str);
}

FrameTimeCode::FrameTimeCode(double timecode_num, double fps) : framerate_{0.0}, frame_num_{0} {
    framerate_ = fps;
    frame_num_ = _parse_timecode_number(timecode_num);
}

FrameTimeCode::FrameTimeCode(int timecode_num, double fps) : framerate_{0.0}, frame_num_{0} {
    framerate_ = fps;
    frame_num_ = _parse_timecode_number(timecode_num);
}

const long long int FrameTimeCode::_parse_timecode_string(const std::string& timecode_str) {
    /*
    Parse a string into the exact number of frames.
    Valid timestamps:
          - 00:05:00.000
          - 00:05:00
          - 9000
          - 300s
          - 300.0
     */
    if (std::isdigit(timecode_str)) {
        timecode = static_cast<long long int>(timecode_str);
        if (timecode_str < 0) {
            throw runtime_error("Timecode frame number must be positive.");
        }
        return timecode;
    }

    auto sep_found = std::find(str.begin(), str.end(), ':');
    if (sep_found != timecode_str.end()) {
        auto hr_min_sec = _parse_hrs_mins_secs_to_second(timecode_str);
        hrs = hr_min_sec.hrs;
        mins = hr_min_sec.mins;
        secs = hr_min_sec.secs;
        if (!(hrs >= 0 && mins >= 0 && secs >= 0 && mins < 60 and secs < 60)) {
            throw runtime_error("Invalid timecode range (values outside allowed range).");
        }
        secs += (hrs * 60 * 60) + (mins * 60);
        // TODO
        // return _seconds_to_frames(secs);
        return secs;
    }
}

const long long int FrameTimeCode::_parse_timecode_number(const int timecode_num) {
}

const long long int FrameTimeCode::_parse_timecode_number(const double timecode_num) {
}

const HourMinSec _parse_hrs_mins_secs_to_second(const std::string& timecode_str) const {
    HourMinSec hrs_min_sec;
    std::vector<std::string> tokens;
    std::string token;
    char delimiter = ':';

    for (char ch : timecode_str) {
        if (ch == delimiter) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += ch;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    if (tokens.size() != 3) {
        throw runtime_error("Invalid timestamp format. It should be like HH:MM:SS.MSS or HH:MM:SS.");
    }

    hrs_min_sec.hrs = static_cast<long long int>(tokens[0]);
    hrs_min_sec.mins = static_cast<int>(tokens[1]);
    hrs_min_sec.secs = static_cast<int>(tokens[2]);
    
    return hrs_min_sec;
}
