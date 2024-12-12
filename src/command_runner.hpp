#ifndef COMMAND_RUNNER_H
#define COMMAND_RUNNER_H

#include "frame_timecode.hpp"

#include <string>
#include <vector>
#include <tuple>
#include <cstdint>

namespace command_runner {

using FrameTimeCodePair = std::tuple<frame_timecode::FrameTimeCode, frame_timecode::FrameTimeCode>;

class CommandRunner {
    public:
        CommandRunner(const std::string& input_path, const std::string& command,
                      const std::string& output_path, const std::vector<FrameTimeCodePair>& scene_list);
        int8_t execute() const;

    private:
        // _split_video();
        int8_t _list_scenes() const;

        const std::string& input_path_;
        const std::string& command_;
        const std::string& output_path_;
        const std::vector<FrameTimeCodePair>& scene_list_;

};

}

#endif