#ifndef COMMAND_RUNNER_H
#define COMMAND_RUNNER_H

#include "frame_timecode.hpp"
#include "error.hpp"

#include <string>
#include <vector>
#include <tuple>
#include <cstdint>

using FrameTimeCodePair = std::tuple<FrameTimeCode, FrameTimeCode>;

class CommandRunner {
    public:
        CommandRunner(const std::string& input_path, const std::string& command,
                      const std::string& output_path, const std::vector<FrameTimeCodePair>& scene_list);
        WithError<void> execute() const;

    private:
        WithError<void> _list_scenes() const;
        WithError<void> _save_images() const;
        WithError<void> _split_video() const;
        WithError<std::string> _splitext() const;

        const std::string& input_path_;
        const std::string& command_;
        const std::string& output_path_;
        const std::vector<FrameTimeCodePair>& scene_list_;
};

#endif