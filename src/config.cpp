#include "config.h"


static int construct_cfg(Config & cfg, argparse::ArgumentParser & program)
{
    const std::filesystem::path input_path(program.get<std::string>("--input"));
    const std::filesystem::path output_dir(program.get<std::string>("--output"));
    const std::string command = program.get<std::string>("--command");

    cfg.input_path = input_path;
    cfg.output_dir = output_dir;
    cfg.command = cfg.command;
    return 0;
}


int parse_args(Config & cfg, int argc, char *argv[])
{
    argparse::ArgumentParser program("shutoh", "0.0.1");

    /* Mandatory */ 
    program.add_argument("-i", "--input")
        .help("Input video file.")
        .required();
    
    program.add_argument("-c", "--command")
        .help("Command name. choose one from [list-scenes, split-videos, save-images]")
        .required();

    program.add_argument("-o", "--output")
        .default_value(std::string("."))
        .help("Output directory for created files. if unset, working directory will be used.");

    /**
    program.add_argument("--filename")
        .help("Output filename format to save csv, images, and videos. As with PySceneDetect, you can use "
              "macros like $VIDEO_NAME, $SCENE_NUMBER, $IMAGE_NUMBER. "
              "Default value: "
              "$VIDEO_NAME-scenes.csv (list-scenes), "
              "$VIDEO_NAME-scene-$SCENE_NUMBER (split-video), "
              "$VIDEO_NAME-scene-$SCENE_NUMBER-$IMAGE_NUMBER (save-images).");

    program.add_argument("--no_output_file")
        .default_value(false)
        .implicit_value(true)
        .help("[list-scenes] Print scene list only.");

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

    program.add_argument("--start")
        .help("Time in video to start detection. Default value reperesents the first frame of the video.");

    program.add_argument("--end")
        .help("Time in video to end detection. Default value represents the last frame of the video.");

    program.add_argument("--duration")
        .help("Maximum time in video to process. Default value represents the whole video length. Ignored if --end is set.");
    
    program.add_argument("--detector")
        .default_value(std::string("content"))
        .help("Detector type. Choose from [adaptive, content, hash, histogram, threshold].");

    program.add_argument("--threshold")
        .scan<'g', float>()
        .help("Threshold for scene shot detection. Higher values ignore small changes of scenes in the video.");
    
    program.add_argument("--min_scene_len")
        .default_value(15)
        .scan<'d', int>()
        .help("Minimum scene length (=#frames) in cuts. Higher values ignore abrupt cuts.");
    
    program.add_argument("--window_width")
        .default_value(2)
        .scan<'d', int>()
        .help("[AdaptiveDetector]: Size of window (#frames) before/after to average together to detect deviations from the mean.");
    
    program.add_argument("--min_content_val")
        .default_value(15.0f)
        .scan<'g', float>()
        .help("[AdaptiveDetector]: Minimum threshold (float) that content_val must be over to register as a new scene.");

    program.add_argument("--dct_size")
        .default_value(16)
        .scan<'d', int>()
        .help("[HashDetector]: Square size of low frequency to use for the DCT.");
    
    program.add_argument("--lowpass")
        .default_value(2)
        .scan<'d', int>()
        .help("[HashDetector]: How much high frequency to filter from the DCT. A value of 2 means keeping lower 1/2 frequency data.");
    
    program.add_argument("--bins")
        .default_value(256)
        .scan<'d', int>()
        .help("[HistogramDetector]: Number of bins to use for the histogram.");
    
    program.add_argument("--fade_bias")
        .default_value(0.0f)
        .scan<'g', float>()
        .help("[ThresholdDetector]: Float between -1.0 and +1.0 that represents the percentage of timecode skew for the start of a scene");
    **/

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        fprintf(stderr, "Error: %s\n", err.what());
        return 1;
    }

    return construct_cfg(cfg, program);
}
