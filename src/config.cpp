#include "config.hpp"

#include <argparse/argparse.hpp>

std::optional<CommonConfig> parse_args(int argc, char *argv[]) {
    argparse::ArgumentParser program("shutoh");

    program.add_argument("-i", "--input")
        .help("Input video file.")
        .required();
    
    program.add_argument("-c", "--command")
        .help("Command name. Choose one from [list-scenes, split-videos, save-images]")
        .required();

    program.add_argument("-o", "--output")
        .default_value(std::string("."))
        .help("Output directory for created files. If unset, working directory will be used.");

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return std::nullopt;
    }

    std::string input_path_str = program.get<std::string>("--input");
    std::string command = program.get<std::string>("--command");
    std::string output_path_str = program.get<std::string>("--output");

    std::filesystem::path input_path(input_path_str);
    std::filesystem::path output_path(output_path_str);

    return CommonConfig { input_path, output_path, command };
}