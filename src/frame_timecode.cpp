#include "frame_timecode.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <cctype>
#include <string>
#include <vector>

const double MAX_FPS_DELTA = 1.0 / 100000;
const double _SECONDS_PER_MINUTE = 60.0;
const double _SECONDS_PER_HOUR = 60.0 * _SECONDS_PER_MINUTE;
const double _MINUTES_PER_HOUR = 60.0;

FrameTimeCode::FrameTimeCode(const FrameTimeCode& timecode) : framerate_{0.0}, frame_num_{0} {
    framerate_ = timecode.framerate_;
    frame_num_ = timecode.frame_num_;
}

FrameTimeCode::FrameTimeCode(const std::string& timecode_str, double fps) : framerate_{0.0}, frame_num_{0} {
    if (fps < 0 || fps >= MAX_FPS_DELTA) {
        throw std::runtime_error("Framerate must be positive and greater than zero.");
    }
    framerate_ = fps;
    frame_num_ = _parse_timecode_string(timecode_str);
}

FrameTimeCode::FrameTimeCode(double timecode_num, double fps) : framerate_{0.0}, frame_num_{0} {
    framerate_ = fps;
    frame_num_ = _parse_timecode_number(timecode_num);
}

FrameTimeCode::FrameTimeCode(int32_t timecode_num, double fps) : framerate_{0.0}, frame_num_{0} {
    framerate_ = fps;
    frame_num_ = _parse_timecode_number(timecode_num);
}

const int32_t FrameTimeCode::_parse_timecode_string(const std::string& timecode_str) const {
    /*
    Parse a string into the exact number of frames.
    Valid timestamps:
          - 00:05:00.000
          - 00:05:00
          - 9000
          - 300s
          - 300.0
     */

    // "9000"
    if (std::all_of(timecode_str.cbegin(), timecode_str.cend(), isdigit)) {
        int32_t timecode = static_cast<int32_t>(std::stoi(timecode_str));
        if (timecode < 0) {
            throw std::runtime_error("Timecode frame number must be positive.");
        }
        return timecode;
    }

    // "00:05:00.000" or "00:05:00"
    auto sep_found = std::find(timecode_str.begin(), timecode_str.end(), ':');
    if (sep_found != timecode_str.end()) {
        HourMinSec hr_min_sec = _parse_hrs_mins_secs_to_second(timecode_str);
        if (!(hr_min_sec.hrs >= 0 && hr_min_sec.mins >= 0 && hr_min_sec.secs >= 0 && hr_min_sec.mins < 60 and hr_min_sec.secs < 60)) {
            throw std::runtime_error("Invalid timecode range (values outside allowed range).");
        }
        int32_t secs = (hr_min_sec.hrs * 60 * 60) + (hr_min_sec.mins * 60) + hr_min_sec.secs;
        return _seconds_to_frames(secs * framerate_);
    }

    return 0;
}

const int32_t FrameTimeCode::_parse_timecode_number(const int32_t timecode_num) const {
    return 0;
}

const int32_t FrameTimeCode::_parse_timecode_number(const double timecode_num) const {
    return 0;
}

const HourMinSec FrameTimeCode::_parse_hrs_mins_secs_to_second(const std::string& timecode_str) const {
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
        throw std::runtime_error("Invalid timestamp format. It should be like HH:MM:SS.MSS or HH:MM:SS.");
    }

    try {
        hrs_min_sec.hrs = static_cast<int32_t>(std::stoi(tokens[0]));
        hrs_min_sec.mins = static_cast<int32_t>(std::stoi(tokens[1]));
        hrs_min_sec.secs = static_cast<int32_t>(std::stoi(tokens[2]));
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid input: not a number." << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Invalid input: out of int32_t range." << std::endl;
    }    
    return hrs_min_sec;
}

const int32_t FrameTimeCode::_seconds_to_frames(const double seconds) const {
    return std::round(seconds * framerate_);
}