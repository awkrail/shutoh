#include "command_runner.hpp"

#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>

namespace command_runner {

CommandRunner::CommandRunner(const std::string& input_path, const std::string& command,
                                const std::string& output_path, const std::vector<FrameTimeCodePair>& scene_list) :
                            input_path_{input_path}, command_{command}, output_path_{output_path}, scene_list_{scene_list} {}

int8_t CommandRunner::execute() const {
    int8_t ret;
    if (command_ == "list-scenes") {
        ret = _list_scenes();
    } else {
        return 0;
    }
    return ret;
}

std::string CommandRunner::_splitext() const {
    const size_t dot_pos = input_path_.find_last_of('.');
    const size_t slash_pos = input_path_.find_last_of('/');
    return input_path_.substr(slash_pos + 1, dot_pos - slash_pos - 1);
}

int8_t CommandRunner::_list_scenes() const {
    const std::string filename = _splitext();
    const std::string output_filename = output_path_ + "/" + filename + "-scenes.csv";
    std::ofstream csv_file(output_filename);

    if(!csv_file.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    csv_file << "scene_number,start_time,end_time\n";
    
    for(int scene_number = 0; scene_number < scene_list_.size(); scene_number++) {
        const std::string start_time = std::get<0>(scene_list_[scene_number]).to_string();
        const std::string end_time = std::get<1>(scene_list_[scene_number]).to_string();
        csv_file << scene_number << "," << start_time << "," << end_time << "\n";
    }

    return 0;
}

}