#include "command_runner.hpp"
#include "frame_timecode.hpp"

#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <string_view>

CommandRunner::CommandRunner(const std::string& input_path, const std::string& command,
                                const std::string& output_path, const std::vector<FrameTimeCodePair>& scene_list) :
                            input_path_{input_path}, command_{command}, output_path_{output_path}, scene_list_{scene_list} {}

void CommandRunner::execute() const {
    if (command_ == "list-scenes") {
        _list_scenes();
    } else if (command_ == "save-images") {
        _save_images();
    } else if (command_ == "split-video") {
        _split_video();
    }
}

WithError<std::string> CommandRunner::_splitext() const {
    const std::string error_msg = "Invalid input filename. The input file should have [filename].[ext] or [directory]/[filename].[ext] format.";
    const size_t dot_pos = input_path_.find_last_of('.');

    if (dot_pos == std::string_view::npos) {
        throw std::runtime_error(error_msg);
    }
    const size_t slash_pos = input_path_.find_last_of('/');
    if (slash_pos == std::string_view::npos) {
        return input_path_.substr(0, dot_pos - 1);
    }
    else {
        const size_t length = dot_pos - slash_pos - 1;
        if (length > 0) {
            return input_path_.substr(slash_pos + 1, dot_pos - slash_pos - 1);
        } else {
            throw std::runtime_error(error_msg);
        }
    }
}

void CommandRunner::_list_scenes() const {
    const std::string filename = _splitext();
    const std::string output_filename = output_path_ + "/" + filename + "-scenes.csv";
    std::ofstream csv_file(output_filename);

    if(!csv_file.is_open()) {
        std::runtime_error("Failed to open file for writing: " + output_filename);
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
}

void CommandRunner::_save_images() const {
}

void CommandRunner::_split_video() const {
}