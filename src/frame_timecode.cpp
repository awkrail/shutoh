#include "frame_timecode.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fmt/core.h>

FrameTimeCode::FrameTimeCode(const FrameTimeCode& timecode) : framerate_{0.0}, frame_num_{0} {
    framerate_ = timecode.framerate_;
    frame_num_ = timecode.frame_num_;
}

FrameTimeCode::FrameTimeCode(const int32_t frame_num, const float fps) : framerate_{0.0}, frame_num_{0} {
    framerate_ = fps;
    frame_num_ = frame_num;
}

const WithError<int32_t> FrameTimeCode::parse_timecode_string(const std::string& timecode_str) const {
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
            const std::string error_msg = "Seconds should be larger than 0.";
            return WithError<int32_t> { std::nullopt, Error(ErrorCode::NegativeSecond, error_msg) };
        }
        return WithError<int32_t> { timecode, Error(ErrorCode::Success, "") };
    }

    // "00:05:00.000" or "00:05:00" -> convert them into the number of frames
    auto sep_found = std::find(timecode_str.begin(), timecode_str.end(), ':');
    if (sep_found != timecode_str.end()) {
        const WithError<TimeStamp> timestamp = _parse_hrs_mins_secs_to_second(timecode_str);
        if (timestamp.has_error()) {
            return WithError<int32_t> { std::nullopt, timestamp.error };
        }
        const float secs = calculate_total_seconds(timestamp.value());
        return WithError<int32_t> { _seconds_to_frames(secs * framerate_), Error(ErrorCode::Success, "") };
    }

    const std::string error_msg = "Invalid timestamp format. Accepted format is HH:MM:SS.nnn or HH:MM:SS, but got " + timecode_str;
    return WithError<int32_t> { std::nullopt, Error(ErrorCode::InvalidTimestamp, error_msg) };
}

int32_t FrameTimeCode::parse_timecode_number(Numeric auto seconds) const {
    return _seconds_to_frames(seconds);
}

std::string FrameTimeCode::to_string() const {
    float secs = static_cast<float>(frame_num_ / framerate_);
    int32_t hrs = static_cast<int32_t>(secs / frame_timecode::_SECONDS_PER_HOUR);
    secs -= (hrs * frame_timecode::_SECONDS_PER_HOUR);
    int32_t mins = static_cast<int32_t>(secs / frame_timecode::_SECONDS_PER_MINUTE);
    secs = std::max(0.0f, secs - (mins * frame_timecode::_SECONDS_PER_MINUTE));
    secs = std::round(secs * 1000) / 1000; // equivalent to round(secs, precison=3) in Python
    secs = std::min(frame_timecode::_SECONDS_PER_MINUTE, secs);
    if (static_cast<int32_t>(secs) == frame_timecode::_SECONDS_PER_MINUTE) {
        secs = 0.0f;
        mins += 1;
        if (mins >= frame_timecode::_MINUTES_PER_HOUR) {
            mins = 0;
            hrs += 1;
        }
    }
    std::string datetime_str = convert_timecode_to_datetime(hrs, mins, secs);
    return datetime_str;
}

const WithError<TimeStamp> FrameTimeCode::_parse_hrs_mins_secs_to_second(const std::string& timecode_str) const {
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
            if (!std::isdigit(ch)) {
                const std::string error_msg = "Invalid timestamp format. The timestamp string characters should be 0 - 9 values.";
                return WithError<TimeStamp> { std::nullopt, Error(ErrorCode::InvalidTimestamp, error_msg) };
            }
            token += ch;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    if (tokens.size() != 3) {
        const std::string error_msg = "Invalid timestamp format. The format should be HH:MM:SS.nnn or HH:MM:SS.";
        return WithError<TimeStamp> { std::nullopt, Error(ErrorCode::InvalidTimestamp, error_msg) };
    }

    /*
        Unfortunately, without stoi/stof, converting string into numbers is very difficult.
        Hence, we remain try - catch in this code. If you have any ideas to remove try - catch, please let me know!
    */
    int32_t hour_val = 0;
    int32_t minute_val = 0;
    float second_val = 0;
    try {
        hour_val = static_cast<int32_t>(std::stoi(tokens[0]));
        minute_val = static_cast<int32_t>(std::stoi(tokens[1]));
        second_val = static_cast<float>(std::stof(tokens[2]));
    } catch (const std::exception& e) {
        const std::string error_msg = "Invalid timestamp format. Failed to convert hours, minutes, seconds into numbers.";
        return WithError<TimeStamp> { std::nullopt, Error(ErrorCode::InvalidTimestamp, error_msg) };
    }

    WithError<Hour> hour = Hour::create_hour(hour_val);
    WithError<Minute> minute = Minute::create_minute(minute_val);
    WithError<Second> second = Second::create_second(second_val);

    if (hour.has_error()) {
        return WithError<TimeStamp> { std::nullopt, hour.error };
    }
    if (minute.has_error()) {
        return WithError<TimeStamp> { std::nullopt, minute.error };
    }
    if (second.has_error()) {
        return WithError<TimeStamp> { std::nullopt, second.error };
    }

    return WithError<TimeStamp> { TimeStamp(hour.value(), minute.value(), second.value()), 
        Error(ErrorCode::Success, "") };
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

bool FrameTimeCode::operator<(const FrameTimeCode& other) const {
    return frame_num_ < other.frame_num_;
}

bool FrameTimeCode::operator>(const FrameTimeCode& other) const {
    return frame_num_ > other.frame_num_;
}

bool FrameTimeCode::operator<=(const FrameTimeCode& other) const {
    return frame_num_ <= other.frame_num_;
}

bool FrameTimeCode::operator>=(const FrameTimeCode& other) const {
    return frame_num_ >= other.frame_num_;
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

namespace frame_timecode {

const float MIN_FPS_DELTA = 1.0 / 100000;
const float _SECONDS_PER_MINUTE = 60.0;
const float _SECONDS_PER_HOUR = 60.0 * _SECONDS_PER_MINUTE;
const float _MINUTES_PER_HOUR = 60.0;

const WithError<FrameTimeCode> from_timecode_string(const std::string& timecode_str, const float fps) {
    if (fps < MIN_FPS_DELTA) {
        const std::string error_msg = "Framerate should be larger than MIN_FPS_DELTA = " + std::to_string(frame_timecode::MIN_FPS_DELTA);
        return WithError<FrameTimeCode> { std::nullopt, Error(ErrorCode::TooSmallFpsValue, error_msg) };
    }
    FrameTimeCode frame_timecode = FrameTimeCode(0, fps);
    const WithError<int32_t> frame_num = frame_timecode.parse_timecode_string(timecode_str);
    if (frame_num.has_error()) {
        return WithError<FrameTimeCode> { std::nullopt, frame_num.error };
    }
    return WithError<FrameTimeCode> { FrameTimeCode(frame_num.value(), fps), Error(ErrorCode::Success, "") };
}

const WithError<FrameTimeCode> from_frame_nums(const int32_t frame_num, const float fps) {
    /*
        Process the timecode value as an exact number of frames.
    */
    if (fps < MIN_FPS_DELTA) {
        const std::string error_msg = "Framerate should be larger than MIN_FPS_DELTA = " + std::to_string(frame_timecode::MIN_FPS_DELTA);
        return WithError<FrameTimeCode> { std::nullopt, Error(ErrorCode::TooSmallFpsValue, error_msg) };
    }

    if (frame_num < 0) {
        const std::string error_msg = "Frame num should be larger than 0, but got " + std::to_string(frame_num);
        return WithError<FrameTimeCode> { std::nullopt, Error(ErrorCode::NegativeFrameNum, error_msg) };
    }
    return WithError<FrameTimeCode> { FrameTimeCode(frame_num, fps), Error(ErrorCode::Success, "") };
}

const WithError<FrameTimeCode> from_seconds(Numeric auto seconds, const float fps) {
    /*
        Conver the number of seconds S (float) into the number of frames.
    */
   if (fps < MIN_FPS_DELTA) {
        const std::string error_msg = "Framerate should be larger than MIN_FPS_DELTA = " + std::to_string(frame_timecode::MIN_FPS_DELTA);
        return WithError<FrameTimeCode> { std::nullopt, Error(ErrorCode::TooSmallFpsValue, error_msg) };
    }

    if (seconds < 0) {
        const std::string error_msg = "Seconds should be larger than 0, but got " + std::to_string(seconds);
        return WithError<FrameTimeCode> { std::nullopt, Error(ErrorCode::NegativeSecond, error_msg) };
    }
    FrameTimeCode frame_timecode = FrameTimeCode(0, fps);
    const int32_t frame_num = frame_timecode.parse_timecode_number(seconds);
    return WithError<FrameTimeCode> { FrameTimeCode(frame_num, fps), Error(ErrorCode::Success, "")  };
}

}

const std::string convert_timecode_to_datetime(const int32_t hrs, const int32_t mins, const float secs) {
    const int32_t int_sec = static_cast<int32_t>(secs);
    const float frac_part = secs - int_sec;
    
    const std::string frac_part_str = std::to_string(frac_part).substr(2, 3);
    const std::string hh_mm_ss_nnn = fmt::format("{:02}:{:02}:{:02}.{}", hrs, mins, int_sec, frac_part_str);
    return hh_mm_ss_nnn;
}

const float calculate_total_seconds(const TimeStamp& timestamp) {
    const int32_t hour = timestamp.hour.get_hour();
    const int32_t minute = timestamp.minute.get_minute();
    const float second = timestamp.second.get_second();
    const float total_second = (hour * frame_timecode::_SECONDS_PER_HOUR) + (minute * frame_timecode::_SECONDS_PER_MINUTE) + second;
    return total_second;
}