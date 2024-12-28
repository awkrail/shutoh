#include "command_runner.hpp"
#include "frame_timecode.hpp"
#include "error.hpp"
#include "csv_writer.hpp"

#include <iostream>
#include <future>
#include <fmt/core.h>

CommandRunner::CommandRunner(const std::string& command, const std::filesystem::path& input_path,
                             const std::filesystem::path& output_dir, const std::vector<FrameTimeCodePair>& scene_list) 
                            : command_{command}, input_path_{input_path}, output_dir_{output_dir}, scene_list_{scene_list} {}

WithError<void> CommandRunner::execute() const {
    if (command_ == "list-scenes") {
        return _list_scenes();
    } else if (command_ == "save-images") {
        return _save_images();
    } else {
        return _split_video();
    }
}

WithError<void> CommandRunner::_list_scenes() const {
    const CSVWriter csv_writer = CSVWriter(output_dir_);
    const std::string filename = input_path_.stem().string();
    const WithError<void> err = csv_writer.write_scenes_to_csv(filename, scene_list_);
    return err;
}

WithError<void> CommandRunner::_save_images() const {
    return WithError<void> { Error(ErrorCode::Success, "") };
}

WithError<void> CommandRunner::_split_video() const {
    std::vector<std::string> commands;
    
    for (size_t scene_number = 0; scene_number < scene_list_.size(); scene_number++) {
        const std::string output_filename = fmt::format("{}/{}-{}.mp4", output_dir_.string(), input_path_.stem().string(), scene_number);
        const FrameTimeCode& start_time = std::get<0>(scene_list_[scene_number]);
        const FrameTimeCode& end_time = std::get<1>(scene_list_[scene_number]);

        const std::string& start_time_str = start_time.to_string_second();
        const std::string& duration_str = (end_time - start_time).to_string_second();

        const std::string command = fmt::format("ffmpeg -nostdin -y -ss {} -i {} -t {} -v quiet "
            "-map 0:v:0 -map 0:a? -map 0:s? -c:v libx264 -preset veryfast -crf 22 -c:a aac -sn {}",
            start_time_str, input_path_.string(), duration_str, output_filename);

        commands.push_back(command);
    }

    /* save scenes for videos with ffmpeg and multi-processing. */
    std::vector<std::future<void>> futures;
    const size_t max_concurrent_tasks = std::thread::hardware_concurrency();

    for (size_t i = 0; i < commands.size(); i++) {
        futures.emplace_back(std::async(std::launch::async, cut_video_with_ffmpeg, commands[i]));
        if (futures.size() >= max_concurrent_tasks) {
            futures.front().get();
            futures.erase(futures.begin());
        }
    }

    /* Remove left processes */
    for (auto& future : futures) {
        future.get();
    }

    return WithError<void> { Error(ErrorCode::Success, "") };
}

WithError<CommandRunner> CommandRunner::initialize_command_runner(const std::filesystem::path& input_path,
                                                                  const std::string& command,
                                                                  const std::filesystem::path& output_dir,
                                                                  const std::vector<FrameTimeCodePair>& scene_list) 
{
    if (!(command == "list-scenes" || command == "split-video" || command == "save-images")) {
        std::string error_msg = "Invalid command. Command should be list-scenes, split-video, or save-images.";
        return WithError<CommandRunner> { std::nullopt, Error(ErrorCode::InvalidCommand, error_msg) };
    }

    CommandRunner command_runner = CommandRunner(command, input_path, output_dir, scene_list);
    return WithError<CommandRunner> { command_runner, Error(ErrorCode::Success, "") };
}

void cut_video_with_ffmpeg(const std::string& command) {
    const int8_t ret = std::system(command.c_str());
    if (ret != 0) {
        std::cerr << "Error: Failed to process a video cut with ffmpeg." << std::endl;
    }
}