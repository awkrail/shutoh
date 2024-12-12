#include "command_runner.hpp"

#include <string>
#include <vector>
#include <tuple>
#include <iostream>

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

int8_t CommandRunner::_list_scenes() const {
    return 0;
}

}