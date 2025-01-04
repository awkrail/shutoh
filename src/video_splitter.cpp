#include "video_splitter.hpp"
#include "frame_timecode.hpp"
#include "error.hpp"

#include <future>
#include <iostream>
#include <fmt/core.h>
#include <regex>

VideoSplitter::VideoSplitter(const std::filesystem::path& output_dir, const std::string& output_filename,
                             const int32_t crf, const std::string& preset, const std::string& ffmpeg_args) 
    : output_dir_{output_dir}, output_filename_{output_filename}, crf_{crf}, preset_{preset}, ffmpeg_args_{ffmpeg_args} {};

void VideoSplitter::split_video(const std::filesystem::path& input_path,
                                const std::vector<FrameTimeCodePair>& scene_list) const {
    const std::vector<std::string> commands = _get_ffmpeg_split_commands(input_path, scene_list);
    _run_ffmpeg_parallel(commands);
}

std::vector<std::string> VideoSplitter::_get_ffmpeg_split_commands(const std::filesystem::path& input_path,
                                                                   const std::vector<FrameTimeCodePair>& scene_list) const {
    const std::string output_dir_str = output_dir_.string();
    const std::string input_path_str = input_path.string();
    const std::regex pattern("@SCENE_NUMBER");

    std::vector<std::string> commands;    
    
    for (size_t scene_number = 0; scene_number < scene_list.size(); scene_number++) {
        
        const std::string output_filename = std::regex_replace(output_filename_, pattern, std::to_string(scene_number));
        
        const FrameTimeCode& start_time = std::get<0>(scene_list[scene_number]);
        const FrameTimeCode& end_time = std::get<1>(scene_list[scene_number]);

        const std::string& start_time_str = start_time.to_string_second();
        const std::string& duration_str = (end_time - start_time).to_string_second();

        const std::string command = fmt::format("ffmpeg -nostdin -y -ss {} -i {} -t {} -v quiet -c:v libx264 -preset {} -crf {} {} {}/{}.mp4",
                                                start_time_str, input_path_str, duration_str, preset_, crf_,
                                                ffmpeg_args_, output_dir_str, output_filename);
        commands.push_back(command);
    }

    return commands;
}

void VideoSplitter::_run_ffmpeg_parallel(const std::vector<std::string>& commands) const {
    std::vector<std::future<void>> futures;
    const size_t max_concurrent_tasks = std::thread::hardware_concurrency();

    for (size_t i = 0; i < commands.size(); i++) {
        futures.emplace_back(std::async(std::launch::async, cut_video_with_ffmpeg, commands[i]));
        if (futures.size() >= max_concurrent_tasks) {
            futures.front().get();
            futures.erase(futures.begin());
        }
    }
    for (auto& future : futures) {
        future.get();
    }
}

void cut_video_with_ffmpeg(const std::string& command) {
    const int8_t ret = std::system(command.c_str());
    if (ret != 0) {
        std::cerr << "Error: Failed to process a video cut with ffmpeg." << std::endl;
    }
}