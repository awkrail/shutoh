#include "config.hpp"
#include "error.hpp"
#include "video_stream.hpp"

WithError<Config> _construct_config(argparse::ArgumentParser& program) {
    const std::string input_path_str = program.get<std::string>("--input");
    const std::string output_path_str = program.get<std::string>("--output");
    const std::string command = program.get<std::string>("--command");
    const std::string filename = program.get<std::string>("--filename");
    const bool verbose = program.get<bool>("--verbose");

    const bool no_output_file = program.get<bool>("--no_output_file");
    
    const bool copy = program.get<bool>("--copy");
    const int32_t crf = program.get<int32_t>("--crf");
    const std::string preset = program.get<std::string>("--preset");
    const std::string ffmpeg_args = program.get<std::string>("--ffmpeg_args");

    const int32_t num_images = program.get<int32_t>("--num_images");
    const std::string format = program.get<std::string>("--format");
    const int32_t quality = program.get<int32_t>("--quality");
    const int32_t frame_margin = program.get<int32_t>("--frame_margin");
    
    std::optional<float> scale = program.present<float>("--scale");
    std::optional<int32_t> height = program.present<int32_t>("--height");
    std::optional<int32_t> width = program.present<int32_t>("--width");

    std::optional<std::string> start = program.present<std::string>("--start");
    std::optional<std::string> end = program.present<std::string>("--end");
    std::optional<std::string> duration = program.present<std::string>("--duration");

    const float threshold = program.get<float>("--threshold");
    const int32_t min_scene_len = program.get<int32_t>("--min_scene_len");

    /* validate arguments */
    if (!(command == "list-scenes" || command == "split-video" || command == "save-images")) {
        std::string error_msg = "--command should be list-scenes, split-video, or save-images.";
        return WithError<Config> { std::nullopt, Error(ErrorCode::InvalidArgument, error_msg) };
    }

    if (crf < 0 || crf > 100) {
        std::string error_msg = "--crf should be 0 < crf < 100.";
        return WithError<Config> { std::nullopt, Error(ErrorCode::InvalidArgument, error_msg) };
    }

    if (!(preset == "ultrafast" || preset == "superfast" || preset == "veryfast" ||
          preset == "faster"    || preset == "fast"      || preset == "medium"   || 
          preset == "slow"      || preset == "slower"    || preset == "veryslow")) {
        std::string error_msg = "--preset should be ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, and veryslow.";
        return WithError<Config> { std::nullopt, Error(ErrorCode::InvalidArgument, error_msg) };
    }

    if (num_images < 0 || num_images > 100) {
        std::string error_msg = "--num_images should be 0 < num_images < 100.";
        return WithError<Config> { std::nullopt, Error(ErrorCode::InvalidArgument, error_msg) };
    }

    if (!(format == "jpg" || format == "png" || format == "webp")) {
        std::string error_msg = "--format should be jpg, png, and webp.";
        return WithError<Config> { std::nullopt, Error(ErrorCode::InvalidArgument, error_msg) };
    }

    if (quality < 0 || quality > 100) {
        std::string error_msg = "--quality should be 0 < quality < 100.";
        return WithError<Config> { std::nullopt, Error(ErrorCode::InvalidArgument, error_msg) };
    }

    if (frame_margin < 0 || frame_margin > 50) {
        std::string error_msg = "--frame_margin should be 0 < frame_margin < 50.";
        return WithError<Config> { std::nullopt, Error(ErrorCode::InvalidArgument, error_msg) };
    }

    if (scale.has_value() && (scale.value() < 0 || scale.value() > 10.0)) {
        std::string error_msg = "--scale should be 0 < scale < 2.0";
        return WithError<Config> { std::nullopt, Error(ErrorCode::InvalidArgument, error_msg) };
    }

    if (height.has_value() && (height.value() < 0 || height.value() > 10000)) {
        std::string error_msg = "--height should be 0 < height < 10000";
        return WithError<Config> { std::nullopt, Error(ErrorCode::InvalidArgument, error_msg) };
    }

    if (width.has_value() && (width.value() < 0 || width.value() > 10000)) {
        std::string error_msg = "--width should be 0 < width < 10000";
        return WithError<Config> { std::nullopt, Error(ErrorCode::InvalidArgument, error_msg) };
    }

    if (threshold < 0.0) {
        std::string error_msg = "--threshold should be positive.";
        return WithError<Config> { std::nullopt, Error(ErrorCode::InvalidArgument, error_msg) };
    }    

    if (min_scene_len < 0) {
        std::string error_msg = "--min_scene_len should be positive.";
        return WithError<Config> { std::nullopt, Error(ErrorCode::InvalidArgument, error_msg) };
    }

    const std::filesystem::path input_path(input_path_str);
    const std::filesystem::path output_dir(output_path_str);

    const Config config = { .input_path = input_path, .output_dir = output_dir,
                            .command = command,       .filename = filename,
                            .verbose = verbose,       .no_output_file = no_output_file,
                            .copy = copy,             .crf = crf,
                            .preset = preset,         .ffmpeg_args = ffmpeg_args,
                            .num_images = num_images, .format = format,
                            .quality = quality,       .frame_margin = frame_margin,
                            .scale = scale,           .height = height,
                            .width = width,           .start = start,
                            .end = end,               .duration = duration,
                            .threshold = threshold,   .min_scene_len = min_scene_len };
    
    return WithError<Config> { config, Error(ErrorCode::Success, "") };
}

WithError<Config> parse_args(int argc, char *argv[]) {
    argparse::ArgumentParser program("shutoh");

    /* General parameters */ 
    program.add_argument("-i", "--input")
        .help("Input video file.")
        .required();
    
    program.add_argument("-c", "--command")
        .help("Command name. choose one from [list-scenes, split-videos, save-images]")
        .required();

    program.add_argument("-o", "--output")
        .default_value(std::string("."))
        .help("Output directory for created files. if unset, working directory will be used.");

    program.add_argument("--filename")
        .default_value(std::string(""))
        .help("Output filename format to save csv, images, and videos. As with PySceneDetect, you can use "
              "macros like $VIDEO_NAME, $SCENE_NUMBER, $IMAGE_NUMBER. "
              "Default value: "
              "$VIDEO_NAME-scenes.csv (list-scenes), "
              "$VIDEO_NAME-scene-$SCENE_NUMBER (split-video), "
              "$VIDEO_NAME-scene-$SCENE_NUMBER-$IMAGE_NUMBER (save-images).");
    
    program.add_argument("--verbose")
        .default_value(false)
        .implicit_value(true)
        .help("Show detailed information.");

    /* command: list-scenes */
    program.add_argument("--no_output_file")
        .default_value(false)
        .implicit_value(true)
        .help("[list-scenes] Print scene list only.");

    /* command: split-video */
    program.add_argument("--copy")
        .default_value(false)
        .implicit_value(true)
        .help("[split-video] Copy instead of re-encode. Faster but less precise.");

    program.add_argument("--crf")
        .default_value(22)
        .scan<'d', int>()
        .help("[split-video] Video encoding quality from 0 to 100, where lower is high quality. 0 is lossless.");

    program.add_argument("--preset")
        .default_value(std::string("veryfast"))
        .help("[split-video] Video compression quality."
              "Choose one from ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow.");

    program.add_argument("--ffmpeg_args")
        .default_value(std::string("-c:a aac -map 0:v:0 -map 0:a? -sn"))
        .help("[split-video] Codec arguments passed to FFmpeg when splitting scenes."
              "Use double quotes around arguments. Must specify at least audio/video codec.");

    /* command: save-images */
    program.add_argument("--num_images")
        .default_value(3)
        .scan<'d', int>()
        .help("[save-images] Number of images to generate per scene."
              "Will always include start/end frame, unless -n=1,"
              "in which case the image will be the frame at the mid-point of the scene.");
    
    program.add_argument("--format")
        .default_value(std::string("jpg"))
        .help("[save-images] Output format of saved images. Choose one from jpg, png, and webp.");
    
    program.add_argument("--quality")
        .default_value(95)
        .scan<'d', int>()
        .help("[save-images] JPEG/WebP encoding quality, from 0 to 100.");
    
    program.add_argument("--compression")
        .default_value(3)
        .scan<'d', int>()
        .help("[save-images] PNG compression rate, from 0-9."
              "Higher values produce smaller files but result in longer compression time.");

    program.add_argument("--frame_margin")
        .default_value(1)
        .scan<'d', int>()
        .help("[save-images] Number of frames to ignore at beginning/end of scenes when saving images."
              "Controls temporal padding on scene boundaries.");

    program.add_argument("--scale")
        .scan<'g', float>()
        .help("[save-images] Factor to scale images. Ignored if -W/--width or -H/--height is set.");

    program.add_argument("-W", "--width")
        .scan<'d', int>()
        .help("[save-images] Width of images.");
    
    program.add_argument("-H", "--height")
        .scan<'d', int>()
        .help("[save-images] Height of images.");

    /* Time */
    program.add_argument("--start")
        .help("Time in video to start detection. Default value reperesents the first frame of the video.");

    program.add_argument("--end")
        .help("Time in video to end detection. Default value represents the last frame of the video.");

    program.add_argument("--duration")
        .help("Maximum time in video to process. Default value represents the whole video length. Ignored if --end is set.");
    
    /* TODO: detector parameters */
    program.add_argument("--threshold")
        .default_value(27.0f)
        .scan<'g', float>()
        .help("Threshold for scene shot detection. Higher values ignore small changes of scenes in the video.");
    
    program.add_argument("--min_scene_len")
        .default_value(15)
        .scan<'d', int>()
        .help("Minimum scene length (=#frames) in cuts. Higher values ignore abrupt cuts.");

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::string error_msg = err.what();
        return WithError<Config> { std::nullopt, Error(ErrorCode::FailedToParseArgs, error_msg) };
    }

    return _construct_config(program);
}

void update_config_with_video(Config& cfg, const VideoStream& video) {
    if (cfg.command != "save-images")
        return;
    
    if (cfg.width.has_value() && cfg.height.has_value()) {
        cfg.resize = ResizeMode::RESIZE_TARGET;
        return;
    }

    if (cfg.width.has_value() || cfg.height.has_value()) {
        auto [resized_width, resized_height] = _calculate_resized_size(video, cfg.width, cfg.height);
        cfg.resize = ResizeMode::RESIZE_TARGET;

        if (!cfg.width.has_value())
            cfg.width = resized_width;
        
        if (!cfg.height.has_value())
            cfg.height = resized_height;
    }

    if (cfg.scale.has_value()) {
        cfg.resize = ResizeMode::RESIZE_SCALE;
    }
}

std::pair<int32_t, int32_t> _calculate_resized_size(const VideoStream& video, 
                                                    std::optional<int32_t> width, 
                                                    std::optional<int32_t> height) {
    if (width.has_value()) {
        float factor = static_cast<float>(*width) / video.width();
        return {*width, static_cast<int32_t>(factor * video.height())};

    } else if (height.has_value()) {
        float factor = static_cast<float>(*height) / video.height();
        return {static_cast<int32_t>(factor * video.width()), *height};
    }
    
    return {video.width(), video.height()};
}