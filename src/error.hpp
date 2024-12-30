#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <optional>

enum class ErrorCode {
    Success,
    NoSuchFile,
    FailedToOpenFile,
    FailedToSetFramePosition,
    NotSupportedCodec,
    TooSmallFpsValue,
    NegativeSecond,
    NegativeFrameNum,
    OverMaximumFrameNum,
    InvalidTimestamp,
    InvalidCommand,
    TimeOutOfRange,
    FunctionIsNotCalled,
};

class Error {
    public:
        Error(const ErrorCode error_code, const std::string& error_msg) : error_code_{error_code}, error_msg_{error_msg} {};
        ErrorCode get_error_code() const { return error_code_; }
        void show_error_msg() const { std::cerr << "Error: " << error_msg_ << std::endl; }

    private:
        ErrorCode error_code_;
        const std::string error_msg_;
};

template <class T>
struct WithError {
    std::optional<T> val = std::nullopt;
    Error error;
    bool has_error() const { return error.get_error_code() != ErrorCode::Success; }
    T value() const { return val.value(); } /* Note that this function is called after nullopt check. */
};

template <>
struct WithError<void> {
    Error error;
    bool has_error() const { return error.get_error_code() != ErrorCode::Success; }
};

#endif