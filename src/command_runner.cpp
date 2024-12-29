#include "command_runner.hpp"
#include "frame_timecode.hpp"
#include "error.hpp"
#include "csv_writer.hpp"
#include "video_splitter.hpp"
#include "image_extractor.hpp"

CommandRunner::CommandRunner(const std::string& command, const std::filesystem::path& input_path,
                             const std::filesystem::path& output_dir, const std::vector<FrameTimeCodePair>& scene_list) 
                            : command_{command}, input_path_{input_path}, output_dir_{output_dir}, scene_list_{scene_list} {}

WithError<void> CommandRunner::execute() const {
    if (command_ == "list-scenes") {
        const CSVWriter csv_writer = CSVWriter(output_dir_);
        return csv_writer.write_scenes_to_csv(input_path_, scene_list_);

    } else if (command_ == "split-video") {
        const VideoSplitter video_splitter = VideoSplitter(output_dir_);
        return video_splitter.split_video(input_path_, scene_list_);

    } else {
        const ImageExtractor image_extractor = ImageExtractor(output_dir_);
        return image_extractor.save_images(input_path_, scene_list_);
    }
}

WithError<CommandRunner> CommandRunner::initialize_command_runner(const std::filesystem::path& input_path,
                                                                  const std::string& command,
                                                                  const std::filesystem::path& output_dir,
                                                                  const std::vector<FrameTimeCodePair>& scene_list) {
    if (!(command == "list-scenes" || command == "split-video" || command == "save-images")) {
        std::string error_msg = "Invalid command. Command should be list-scenes, split-video, or save-images.";
        return WithError<CommandRunner> { std::nullopt, Error(ErrorCode::InvalidCommand, error_msg) };
    }

    CommandRunner command_runner = CommandRunner(command, input_path, output_dir, scene_list);
    return WithError<CommandRunner> { command_runner, Error(ErrorCode::Success, "") };
}