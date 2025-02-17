#ifndef CONFIG_H
#define CONFIG_H

#include "shutoh/frame_timecode.hpp"
#include "shutoh/detector/base_detector.hpp"
#include "shutoh/detector/content_detector.hpp"
#include "shutoh/detector/hash_detector.hpp"
#include "shutoh/detector/histogram_detector.hpp"
#include "shutoh/detector/threshold_detector.hpp"
#include "shutoh/detector/adaptive_detector.hpp"

#include <opencv2/opencv.hpp>
#include <filesystem>
#include <optional>
#include <argparse/argparse.hpp>
#include <memory>

class VideoStream;
template <typename T> struct WithError;
struct DetectorParameters;

enum class DetectorType {
    CONTENT,
    HASH,
    HISTOGRAM,
    ADAPTIVE,
    THRESHOLD,
    OTHER,
};

struct Config {
    /* common */
    const std::filesystem::path input_path;
    const std::filesystem::path output_dir;
    const std::string command;
    const std::string filename;

    /* list-scene */
    const bool no_output_file;

    /* split-video */
    const bool copy;
    const int32_t crf;
    const std::string preset;
    const std::string ffmpeg_args;

    /* save-images */
    const int32_t num_images;
    const std::string format;
    const int32_t quality;
    const int32_t frame_margin;
    const std::optional<float> scale; /* scale is ignored if width and height are set. */
    const std::optional<int32_t> height;
    const std::optional<int32_t> width;

    /* time information
       Time expression is represented as string format,
       i.e., HH:MM:SS[.nnn] or HH:MM:SS. e.g., 00:05:00[.000] or 00:05:00
       These values is set to be std::nullopt as default value, 
       representing start = 0(s), end = video_duration(s).
    */
    const std::optional<std::string> start;
    const std::optional<std::string> end;
    const std::optional<std::string> duration;

    /* detectors' common parameters */
    const DetectorType detector_type;
    const float threshold;
    const int32_t min_scene_len = 15;

    /* adaptive detector */
    const int32_t window_width;
    const float min_content_val;

    /* hash detector */
    const int32_t dct_size;
    const int32_t lowpass;

    /* histogram detector */
    const int32_t bins;
    const float fade_bias;
};

std::shared_ptr<BaseDetector> _select_detector(const DetectorParameters& params);
std::string _interpret_filename(const std::filesystem::path& input_path,
                                const argparse::ArgumentParser& program);
DetectorType _convert_name_to_type(const std::string& detector_name);
float _get_default_threshold(const DetectorType& detector_type);
WithError<Config> _construct_config(argparse::ArgumentParser& program);
WithError<Config> parse_args(int argc, char *argv[]);

#endif