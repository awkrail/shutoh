#include "frame_timecode.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <cctype>
#include <string>
#include <vector>

namespace frame_timecode {

const float MAX_FPS_DELTA = 1.0 / 100000;
const float _SECONDS_PER_MINUTE = 60.0;
const float _SECONDS_PER_HOUR = 60.0 * _SECONDS_PER_MINUTE;
const float _MINUTES_PER_HOUR = 60.0;

FrameTimeCode::FrameTimeCode(const FrameTimeCode& timecode) : framerate_{0.0}, frame_num_{0} {
    framerate_ = timecode.framerate_;
    frame_num_ = timecode.frame_num_;
}

FrameTimeCode::FrameTimeCode(const int32_t frame_num, const float fps) : framerate_{0.0}, frame_num_{0} {
    framerate_ = fps;
    frame_num_ = frame_num;
}

int32_t FrameTimeCode::parse_timecode_string(const std::string& timecode_str) const {
    /*
    Parse a string into the exact number of frames.
    framerate should be set before calling this method.
    The strings '00:05:00.000', '00:05:00', '300' and '300.0' are all possible valid values,
    all representing a period of time equal to 5 minutes and 300 seconds.
     */

    // 300
    if (std::all_of(timecode_str.cbegin(), timecode_str.cend(), isdigit)) {
        int32_t timecode = static_cast<int32_t>(std::stoi(timecode_str));
        if (timecode < 0) {
            throw std::runtime_error("Timecode frame number must be positive.");
        }
        return timecode;
    }

    // "00:05:00.000" or "00:05:00" -> convert them into the number of frames
    auto sep_found = std::find(timecode_str.begin(), timecode_str.end(), ':');
    if (sep_found != timecode_str.end()) {
        HourMinSec hr_min_sec = _parse_hrs_mins_secs_to_second(timecode_str);
        if (!(hr_min_sec.hrs >= 0 && hr_min_sec.mins >= 0 && hr_min_sec.secs >= 0 && hr_min_sec.mins < 60 and hr_min_sec.secs < 60)) {
            throw std::runtime_error("Invalid timecode range (values outside allowed range).");
        }
        float secs = (hr_min_sec.hrs * 60 * 60) + (hr_min_sec.mins * 60) + hr_min_sec.secs;
        return _seconds_to_frames(secs * framerate_);
    }

    throw std::invalid_argument("Invalid argument: " + timecode_str);
}

int32_t FrameTimeCode::parse_timecode_number(const int32_t seconds) const {
    if (seconds < 0) {
        throw std::invalid_argument("Timecode frame number must be greater than 0.");
    }
    return _seconds_to_frames(seconds);
}

int32_t FrameTimeCode::parse_timecode_number(const float seconds) const {
    if (seconds < 0.0) {
        throw std::invalid_argument("Timecode frame number must be greater than 0.");
    }
    return _seconds_to_frames(seconds);
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
        throw std::runtime_error("Invalid timestamp format. It should be like HH:MM:SS.MMM or HH:MM:SS.");
    }

    try {
        hrs_min_sec.hrs = static_cast<int32_t>(std::stoi(tokens[0]));
        hrs_min_sec.mins = static_cast<int32_t>(std::stoi(tokens[1]));
        hrs_min_sec.secs = static_cast<float>(std::stof(tokens[2]));
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid input: not a number." << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Invalid input: out of int32_t range." << std::endl;
    }    
    return hrs_min_sec;
}

int32_t FrameTimeCode::_seconds_to_frames(const float seconds) const {
    return std::round(seconds * framerate_);
}

bool FrameTimeCode::operator==(const FrameTimeCode& other) const {
    return framerate_ == other.get_framerate() && frame_num_ == other.get_frame_num();
}

bool FrameTimeCode::operator!=(const FrameTimeCode& other) const {
    return !(framerate_ == other.get_framerate() && frame_num_ == other.get_frame_num());
}

const FrameTimeCode FrameTimeCode::operator+(const FrameTimeCode& other) const {
    if(framerate_ != other.framerate_) {
        throw std::runtime_error("Framerate should be same between operands.");
    }
    return FrameTimeCode(frame_num_ + other.frame_num_, framerate_);
}

const FrameTimeCode FrameTimeCode::operator-(const FrameTimeCode& other) const {
    if(framerate_ != other.framerate_) {
        throw std::runtime_error("Framerate should be same between operands.");
    }
    const int32_t new_frame_num = std::max(frame_num_ - other.frame_num_, 0);
    return FrameTimeCode(new_frame_num, framerate_);
}

const FrameTimeCode from_timecode_string(const std::string& timecode_str, float fps) {
    if (fps < MAX_FPS_DELTA) {
        throw std::invalid_argument("Framerate must be positive and greater than MAX_FPS_DELTA = 1.0 / 100000.");
    }
    FrameTimeCode frame_timecode = FrameTimeCode(0, fps);
    const int32_t frame_num = frame_timecode.parse_timecode_string(timecode_str);
    return FrameTimeCode(frame_num, fps);
}

const FrameTimeCode from_frame_nums(int32_t frame_num, float fps) {
    /*
        Process the timecode value as an exact number of frames.
    */
    if (fps < MAX_FPS_DELTA || frame_num < 0) {
        throw std::invalid_argument("Framerate and frame_num must be positive and greater than 0.");
    }
   return FrameTimeCode(frame_num, fps);
}

const FrameTimeCode from_seconds(float seconds, float fps) {
    /*
        Conver the tumber of seconds S into the number of frames.
    */
    if (fps < MAX_FPS_DELTA || seconds < 0) {
        throw std::invalid_argument("Framerate and seconds and seconds must be positive and greater than 0.");
    }
    FrameTimeCode frame_timecode = FrameTimeCode(0, fps);
    const int32_t frame_num = frame_timecode.parse_timecode_number(seconds);
    return FrameTimeCode(frame_num, fps);
}

const FrameTimeCode from_seconds(int32_t seconds, float fps) {
    /*
        Conver the tumber of seconds S into the number of frames.
    */
    if (fps < MAX_FPS_DELTA || seconds < 0) {
        throw std::invalid_argument("Framerate and seconds must be positive and greater than 0.");
    }
    FrameTimeCode frame_timecode = FrameTimeCode(0, fps);
    const int32_t frame_num = frame_timecode.parse_timecode_number(seconds);
    return FrameTimeCode(frame_num, fps);
}


}