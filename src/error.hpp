#ifndef ERROR_H
#define ERROR_H

#include <system_error>
#include <optional>

enum class ErrorCode {
    Success,
    NoSuchFile,
    FailedToOpenFile,
    NotSupportedCodec,
    TooSmallFpsValue,
    NegativeSecond,
    NegativeFrameNum,
    InvalidTimestamp,
    TimeOutOfRange,
};

class Error {
    public:
        Error(const ErrorCode error_code, const std::string& error_msg) : error_code_{error_code}, error_msg_{error_msg} {};
        const std::string& get_error_msg() const { return error_msg_; }
        const ErrorCode get_error_code() const { return error_code_; }

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

#endif