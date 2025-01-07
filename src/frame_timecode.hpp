#ifndef FRAME_TIMECODE_H
#define FRAME_TIMECODE_H

#include "error.hpp"

#include <string>
#include <cstdint>

constexpr int32_t HOUR_MAX = 10;

class Hour {
    public:
        Hour(const int32_t hour) : hour_{hour} {}
        static WithError<Hour> create_hour(const int32_t hour) {
            if (hour < 0 || hour >= HOUR_MAX) {
                const std::string error_msg = "Hour should be 0 < x < 10, but got " + std::to_string(hour);
                return WithError<Hour> { std::nullopt, Error(ErrorCode::TimeOutOfRange, error_msg) };
            }
            return WithError<Hour> { Hour(hour), Error(ErrorCode::Success, "") };
        }
        const int32_t get_hour() const { return hour_; }
    
    private:
        const int32_t hour_;
};

class Minute {
    public:
        Minute(const int32_t minute) : minute_{minute} {}
        static WithError<Minute> create_minute(const int32_t minute) {
            if (minute < 0 || minute >= 60) {
                const std::string error_msg = "Minute should be 0 < x < 60, but got " + std::to_string(minute);
                return WithError<Minute> { std::nullopt, Error(ErrorCode::TimeOutOfRange, error_msg) };
            }
            return WithError<Minute> { Minute(minute), Error(ErrorCode::Success, "") };
        }
        const int32_t get_minute() const { return minute_; }

    private:
        const int32_t minute_;
};

class Second {
    public:
        Second(const float second) : second_{second} {}
        static WithError<Second> create_second(const float second) {
            if (second < 0 || second >= 60) {
                const std::string error_msg = "Second should be 0 < x < 60, but got " + std::to_string(second);
                return WithError<Second> { std::nullopt, Error(ErrorCode::TimeOutOfRange, error_msg) };
            }
            return WithError<Second> { Second(second), Error(ErrorCode::Success, "") };
        }
        const float get_second() const { return second_; }

    private:
        const float second_;
};

struct TimeStamp {
    Hour hour;
    Minute minute;
    Second second;
};

class FrameTimeCode {
    public:
        FrameTimeCode(const FrameTimeCode& timecode);
        FrameTimeCode(const int32_t frame_num, const float fps);

        float get_framerate() const { return framerate_; }
        int32_t get_frame_num() const { return frame_num_; }

        WithError<int32_t> parse_timecode_string(const std::string& timecode_str) const;
        int32_t parse_timecode_number(const int32_t seconds) const;
        int32_t parse_timecode_number(const float seconds) const;
        std::string to_string() const;
        std::string to_string_second() const;

        bool operator==(const FrameTimeCode& other) const;
        bool operator!=(const FrameTimeCode& other) const;
        bool operator<(const FrameTimeCode& other) const;
        bool operator>(const FrameTimeCode& other) const;
        bool operator<=(const FrameTimeCode& other) const;
        bool operator>=(const FrameTimeCode& other) const;

        FrameTimeCode operator+(const FrameTimeCode& other) const;
        FrameTimeCode operator-(const FrameTimeCode& other) const;

    private:
        WithError<TimeStamp> _parse_hrs_mins_secs_to_second(const std::string& timecode_str) const;

        float framerate_;
        int32_t frame_num_;
};

namespace frame_timecode {

constexpr float MIN_FPS_DELTA = 1.0 / 100000;
constexpr float _SECONDS_PER_MINUTE = 60.0;
constexpr float _SECONDS_PER_HOUR = 60.0 * _SECONDS_PER_MINUTE;
constexpr float _MINUTES_PER_HOUR = 60.0;

WithError<FrameTimeCode> from_timecode_string(const std::string& timecode_str, const float fps);
WithError<FrameTimeCode> from_frame_nums(const int32_t frame_num, const float fps);
WithError<FrameTimeCode> from_seconds(const int32_t seconds, const float fps);
WithError<FrameTimeCode> from_seconds(const float seconds, const float fps);

}

float calculate_total_seconds(const TimeStamp& timestamp);
std::string convert_timecode_to_datetime(const int32_t hrs, const int32_t mins, const float secs);

#endif