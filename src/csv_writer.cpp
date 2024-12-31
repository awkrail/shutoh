#include "csv_writer.hpp"
#include "frame_timecode.hpp"
#include "error.hpp"

#include <fstream>
#include <filesystem>
#include <fmt/core.h>

CSVWriter::CSVWriter(const std::filesystem::path& output_dir) : output_dir_{output_dir} {};

WithError<void> CSVWriter::write_scenes_to_csv(const std::filesystem::path& input_path,
                                               const std::vector<FrameTimeCodePair>& scene_list) const {
    
    const std::string filename = input_path.stem().string();
    const std::string output_csv_file = fmt::format("{}/{}-scenes.csv", output_dir_.string(), filename);
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