#include "command_runner.hpp"
#include "frame_timecode.hpp"
#include "error.hpp"
#include "csv_writer.hpp"
#include "video_splitter.hpp"
#include "image_extractor.hpp"
#include "video_stream.hpp"

CommandRunner::CommandRunner(const Config& cfg, const std::vector<FrameTimeCodePair>& scene_list) 
                            : cfg_{cfg}, scene_list_{scene_list} {}

WithError<void> CommandRunner::execute(VideoStream& video) const {
    if (cfg_.command == "list-scenes")
        return _list_scenes();
    else if (cfg_.command == "split-video")
        return _split_video();
    else
        return _save_images(video);
}

WithError<void> CommandRunner::_list_scenes() const {
    const CSVWriter csv_writer = CSVWriter(cfg_.output_dir, cfg_.filename, cfg_.no_output_file);
    return csv_writer.list_scenes(scene_list_);
}

WithError<void> CommandRunner::_split_video() const {
    const std::string ffmpeg_args = cfg_.copy ? "-map 0:v:0 -map 0:a? -map 0:s? -c:v copy -c:a copy" : cfg_.ffmpeg_args;
    const VideoSplitter video_splitter = VideoSplitter(cfg_.output_dir, cfg_.filename, cfg_.crf, cfg_.preset, ffmpeg_args);
    video_splitter.split_video(cfg_.input_path, scene_list_);
    return WithError<void> { Error(ErrorCode::Success, "") };
}

WithError<void> CommandRunner::_save_images(VideoStream& video) const {
    const ImageExtractor image_extractor = ImageExtractor(cfg_.output_dir, cfg_.filename,
                                                          cfg_.num_images, cfg_.frame_margin,
                                                          cfg_.format, cfg_.quality, cfg_.width,
                                                          cfg_.height, cfg_.scale);
    return image_extractor.save_images(video, scene_list_);
}