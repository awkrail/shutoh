#include "config.hpp"

#include <argparse/argparse.hpp>

std::optional<CommonConfig> parse_args(int argc, char *argv[]) {
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
        .help("Output filename format to save csv, images, and videos. As with PySceneDetect, you can use "
              "macros like $VIDEO_NAME, $SCENE_NUMBER, $IMAGE_NUMBER. "
              "Default value: "
              "$VIDEO_NAME-scenes.csv (list-scenes), "
              "$VIDEO_NAME-scene-$SCENE_NUMBER (split-video), "
              "$VIDEO_NAME-scene-$SCENE_NUMBER-$IMAGE_NUMBER (save-images).");
    
    program.add_argument("--verbose")
        .default_value(false)
        .help("Show detailed information.");

    /* command: list-scenes */
    program.add_argument("--no_output_file")
        .default_value(false)
        .help("[list-scenes] Print scene list only.");

    /* command: split-video */
    program.add_argument("--copy")
        .default_value(false)
        .help("[split-video] Copy instead of re-encode. Faster but less precise.");

    program.add_argument("--crf")
        .default_value(22)
        .help("[split-video] Video encoding quality from 0 to 100, where lower is high quality. 0 is lossless.");

    program.add_argument("--preset")
        .default_value(std::string("veryfast"))
        .help("[split-video] Video compression quality."
              "Choose one from ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow.");

    program.add_argument("--args")
        .default_value(std::string("-c:a aac -map 0:v:0 -map 0:a? -sn"))
        .help("[split-video] Codec arguments passed to FFmpeg when splitting scenes."
              "Use double quotes around arguments. Must specify at least audio/video codec.");

    /* command: save-images */
    program.add_argument("--num_images")
        .default_value(3)
        .help("[save-images] Number of images to generate per scene."
              "Will always include start/end frame, unless -n=1,"
              "in which case the image will be the frame at the mid-point of the scene.");
    
    program.add_argument("--format")
        .default_value(std::string("jpg"))
        .help("[save-images] Output format of saved images. Choose one from jpg, png, and webp.");
    
    program.add_argument("--quality")
        .default_value(95)
        .help("[save-images] JPEG/WebP encoding quality, from 0 to 100.");
    
    program.add_argument("--compression")
        .default_value(3)
        .help("[save-images] PNG compression rate, from 0-9."
              "Higher values produce smaller files but result in longer compression time.");

    program.add_argument("--frame_margin")
        .default_value(3)
        .help("[save-images] Number of frames to ignore at beginning/end of scenes when saving images."
              "Controls temporal padding on scene boundaries.");

    program.add_argument("--scale")
        .default_value(1.0)
        .help("[save-images] Factor to scale images. Ignored if -W/--width or -H/--height is set.");

    program.add_argument("-W", "--width")
        .default_value(0.0)
        .help("[save-images] Width of images.");
    
    program.add_argument("-H", "--height")
        .default_value(0.0)
        .help("[save-images] Height of images.");

    /* Time */
    program.add_argument("--start")
        .default_value("-1")
        .help("Time in video to start detection. Default value is the first frame of the video.");

    program.add_argument("--end")
        .default_value("-1")
        .help("Time in video to end detection. Default value is the last frame of the video.");

    program.add_argument("--duration")
        .default_value("-1")
        .help("Maximum time in video to process. Ignored if --end is set.");
    
    /* TODO: detector parameters */

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