#ifndef FRAME_TIMECODE_H
#define FRAME_TIMECODE_H

#include <memory>
#include <string>
#include <cstdint>

#include "error.hpp"

const int32_t HOUR_MAX = 10;

template <typename T>
concept Numeric = std::same_as<T, int32_t> || std::same_as<T, float>;

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
        FrameTimeCode() : frame_num_{0}, framerate_{1.0} {}
        FrameTimeCode(const FrameTimeCode& timecode);
        FrameTimeCode(const int32_t frame_num, const float fps);

        float get_framerate() const { return framerate_; }
        int32_t get_frame_num() const { return frame_num_; }

        void set_framerate(const float framerate) { framerate_ = framerate; }
        void set_frame_num(const int32_t frame_num) { frame_num_ = frame_num; }

        const WithError<int32_t> parse_timecode_string(const std::string& timecode_str) const;
        int32_t parse_timecode_number(const Numeric auto seconds) const;
        std::string to_string() const;

        bool operator==(const FrameTimeCode& other) const;
        bool operator!=(const FrameTimeCode& other) const;
        bool operator<(const FrameTimeCode& other) const;
        bool operator>(const FrameTimeCode& other) const;
        bool operator<=(const FrameTimeCode& other) const;
        bool operator>=(const FrameTimeCode& other) const;

        const FrameTimeCode operator+(const FrameTimeCode& other) const;
        const FrameTimeCode operator-(const FrameTimeCode& other) const;

    private:
        const WithError<TimeStamp> _parse_hrs_mins_secs_to_second(const std::string& timecode_str) const;
        int32_t _seconds_to_frames(const float seconds) const;

        float framerate_;
        int32_t frame_num_;
};

namespace frame_timecode {

extern const float MIN_FPS_DELTA;
extern const float _SECONDS_PER_MINUTE;
extern const float _SECONDS_PER_HOUR;
extern const float _MINUTES_PER_HOUR;

const WithError<FrameTimeCode> from_timecode_string(const std::string& timecode_str, const float fps);
const WithError<FrameTimeCode> from_frame_nums(const int32_t frame_num, const float fps);
const WithError<FrameTimeCode> from_seconds(const Numeric auto seconds, const float fps);

}

const float calculate_total_seconds(const TimeStamp& timestamp);
const std::string convert_timecode_to_datetime(const int32_t hrs, const int32_t mins, const float secs);

#endif