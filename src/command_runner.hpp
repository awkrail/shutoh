#ifndef COMMAND_RUNNER_H
#define COMMAND_RUNNER_H

#include "frame_timecode_pair.hpp"

#include <string>
#include <vector>
#include <tuple>
#include <filesystem>

template <typename T> struct WithError;

class CommandRunner {
    public:
        CommandRunner(const std::string& command, const std::filesystem::path& input_path,
                      const std::filesystem::path& output_dir, const std::vector<FrameTimeCodePair>& scene_list);
        WithError<void> execute() const;
        static WithError<CommandRunner> initialize_command_runner(const std::filesystem::path& input_path,
                                                                  const std::string& command,
                                                                  const std::filesystem::path& output_dir,
                                                                  const std::vector<FrameTimeCodePair>& scene_list);

    private:
        const std::string command_;
        const std::filesystem::path input_path_;
        const std::filesystem::path output_dir_;
        const std::vector<FrameTimeCodePair> scene_list_;
};

void cut_video_with_ffmpeg(const std::string& command);

#endif