#include "parameters.hpp"
#include "config.hpp"

DetectorParameters initialize_parameters(const Config& cfg) {
    const DetectorType detector_type = cfg.detector_type;
    const float threshold = cfg.threshold;
    const int32_t min_scene_len = cfg.min_scene_len;

    switch (detector_type) {
        case DetectorType::CONTENT:
            return DetectorParameters { .detector_type = detector_type, .threshold = threshold, .min_scene_len = min_scene_len };
        case DetectorType::ADAPTIVE: {
            const AdaptiveParameters adaptive_params { .window_width = cfg.window_width, .min_content_val = cfg.min_content_val };
            return DetectorParameters { .detector_type = detector_type, .threshold = threshold, .min_scene_len = min_scene_len, .adaptive_params = adaptive_params };
        }
        case DetectorType::HASH: {
            const HashParameters hash_params { .dct_size = cfg.dct_size, .lowpass = cfg.lowpass };
            return DetectorParameters { .detector_type = detector_type, .threshold = threshold, .min_scene_len = min_scene_len, .hash_params = hash_params };
        }
        case DetectorType::HISTOGRAM: {
            const HistogramParameters histo_params { .bins = cfg.bins };
            return DetectorParameters { .detector_type = detector_type, .threshold = threshold, .min_scene_len = min_scene_len, .histogram_params = histo_params };
        }
        case DetectorType::THRESHOLD: {
            const ThresholdParameters threshold_params { .fade_bias = cfg.fade_bias };
            return DetectorParameters { .detector_type = detector_type, .threshold = threshold, .min_scene_len = min_scene_len, .threshold_params = threshold_params };
        }
        default:
            return DetectorParameters { .detector_type = detector_type, .threshold = threshold, .min_scene_len = min_scene_len };
    }
}
