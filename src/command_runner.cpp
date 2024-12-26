#include "command_runner.hpp"
#include "frame_timecode.hpp"

#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <string_view>
#include <filesystem>
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
    const std::string filename = input_path_.stem().string();
    const std::string output_filename = fmt::format("{}/{}-scenes.csv", output_dir_.string(), filename);
    std::ofstream csv_file(output_filename);

    if(!csv_file.is_open()) {
        const std::string error_msg = "Failed to open csv file for writing: " + output_filename;
        return WithError<void> { Error(ErrorCode::FailedToOpenFile, error_msg) };
    }

    csv_file << "scene_number,start_frame,start_time,end_frame,end_time\n";
    
    for(int scene_number = 0; scene_number < scene_list_.size(); scene_number++) {
        const FrameTimeCode start_time = std::get<0>(scene_list_[scene_number]);
        const FrameTimeCode end_time = std::get<1>(scene_list_[scene_number]);

        const int32_t start_index = start_time.get_frame_num();
        const int32_t end_index = end_time.get_frame_num();

        const std::string start_time_str = start_time.to_string();
        const std::string end_time_str = end_time.to_string();

        csv_file << scene_number << "," << start_index << "," << start_time_str << "," << end_index << "," << end_time_str << "\n";
    }

    csv_file.close();
    return WithError<void> { Error(ErrorCode::Success, "") };
}

WithError<void> CommandRunner::_save_images() const {
    return WithError<void> { Error(ErrorCode::Success, "") };
}

WithError<void> CommandRunner::_split_video() const {
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