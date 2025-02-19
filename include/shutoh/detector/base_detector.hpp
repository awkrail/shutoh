#ifndef BASE_DETECTOR_H
#define BASE_DETECTOR_H

#include <optional>
#include <cstdint>

struct VideoFrame;

class BaseDetector {
    public:
        virtual std::optional<int32_t> process_frame(const VideoFrame& next_frame) = 0;
        virtual ~BaseDetector() {}

    protected:
        BaseDetector() = default;
};

#endif
