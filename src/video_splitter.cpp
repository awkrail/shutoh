#include "video_splitter.hpp"
#include "frame_timecode.hpp"
#include "error.hpp"

#include <future>
#include <iostream>
#include <fmt/core.h>

VideoSplitter::VideoSplitter(const std::filesystem::path& output_dir) : output_dir_{output_dir} {};

WithError<void> VideoSplitter::split_video(const std::filesystem::path& input_path,
                                           const int32_t crf,
                                           const std::string& preset,
                                           const std::string& ffmpeg_args,
                                           const std::vector<FrameTimeCodePair>& scene_list) const {
    
    const std::string output_dir_str = output_dir_.string();
    const std::string input_path_str = input_path.string();
    const std::string filename = input_path.stem().string();

    std::vector<std::string> commands;    
    for (size_t scene_number = 0; scene_number < scene_list.size(); scene_number++) {
        const std::string output_filename = fmt::format("{}/{}-{}.mp4", output_dir_str, filename, scene_number);
        const FrameTimeCode& start_time = std::get<0>(scene_list[scene_number]);
        const FrameTimeCode& end_time = std::get<1>(scene_list[scene_number]);

        const std::string& start_time_str = start_time.to_string_second();
        const std::string& duration_str = (end_time - start_time).to_string_second();

        const std::string command = fmt::format("ffmpeg -nostdin -y -ss {} -i {} -t {} -v quiet -preset {} -crf {} {} {}",
                                                start_time_str, input_path_str, duration_str,
                                                preset, crf, ffmpeg_args, output_filename);
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

void cut_video_with_ffmpeg(const std::string& command) {
    const int8_t ret = std::system(command.c_str());
    if (ret != 0) {
        std::cerr << "Error: Failed to process a video cut with ffmpeg." << std::endl;
    }
}