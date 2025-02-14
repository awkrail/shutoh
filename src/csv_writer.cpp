#include "shutoh/frame_timecode.hpp"
#include "shutoh/error.hpp"

#include "csv_writer.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <fmt/core.h>

CSVWriter::CSVWriter(const std::filesystem::path& output_dir, const std::string& output_filename,
                     const bool no_output_file) : output_dir_{output_dir}, output_filename_{output_filename},
                     no_output_file_{no_output_file} {};

WithError<void> CSVWriter::list_scenes(const std::vector<FrameTimeCodePair>& scene_list) const {
    if (no_output_file_) {
        _print_scenes(scene_list);
        return WithError<void> { Error(ErrorCode::Success, "") };
    }
    else {
        return _write_scenes_to_csv(scene_list);
    }
}

void CSVWriter::_print_scenes(const std::vector<FrameTimeCodePair>& scene_list) const {

    std::cout << "scene_number,start_frame,start_time,end_frame,end_time" << std::endl;

    for(size_t scene_number = 0; scene_number < scene_list.size(); scene_number++) {
        const FrameTimeCode start_time = std::get<0>(scene_list[scene_number]);
        const FrameTimeCode end_time = std::get<1>(scene_list[scene_number]);

        const int32_t start_index = scene_number == 0 ? start_time.get_frame_num() : start_time.get_frame_num() + 1;
        const int32_t end_index = end_time.get_frame_num();

        const std::string start_time_str = start_time.to_string();
        const std::string end_time_str = end_time.to_string();

        std::cout << scene_number << "," << start_index << "," << start_time_str 
            << "," << end_index << "," << end_time_str << std::endl;
    }
}

WithError<void> CSVWriter::_write_scenes_to_csv(const std::vector<FrameTimeCodePair>& scene_list) const {
    const std::string output_csv_file = fmt::format("{}/{}.csv", output_dir_.string(), output_filename_);
    std::ofstream csv_file(output_csv_file);

    if(!csv_file.is_open()) {
        const std::string error_msg = "Failed to open csv file for writing: " + output_csv_file;
        return WithError<void> { Error(ErrorCode::FailedToOpenFile, error_msg) };
    }

    csv_file << "scene_number,start_frame,start_time,end_frame,end_time\n";
    
    for(size_t scene_number = 0; scene_number < scene_list.size(); scene_number++) {
        const FrameTimeCode start_time = std::get<0>(scene_list[scene_number]);
        const FrameTimeCode end_time = std::get<1>(scene_list[scene_number]);

        const int32_t start_index = scene_number == 0 ? start_time.get_frame_num() : start_time.get_frame_num() + 1;
        const int32_t end_index = end_time.get_frame_num();

        const std::string start_time_str = start_time.to_string();
        const std::string end_time_str = end_time.to_string();

        csv_file << scene_number << "," << start_index << "," << start_time_str 
            << "," << end_index << "," << end_time_str << "\n";
    }

    csv_file.close();
    return WithError<void> { Error(ErrorCode::Success, "") };
}