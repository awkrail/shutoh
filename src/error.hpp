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
};

class Error {
    public:
        Error(const ErrorCode error_code, const std::string& error_msg) : error_code_{error_code}, error_msg_{error_msg} {};
        bool has_error() const { return error_code_ != ErrorCode::Success; }
        const std::string& get_error_msg() const { return error_msg_; }

    private:
        ErrorCode error_code_;
        const std::string error_msg_;
};

template <class T>
struct WithError {
    std::optional<T> value = std::nullopt;
    Error error;
};

#endif