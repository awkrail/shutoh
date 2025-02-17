#include <shutoh/detector/adaptive_detector.hpp>
#include <shutoh/detector/content_detector.hpp>
#include <shutoh/detector/hash_detector.hpp>
#include <shutoh/detector/histogram_detector.hpp>
#include <shutoh/detector/threshold_detector.hpp>
#include <pybind11/pybind11.h>

void bind_base_detector(pybind11::module_ &m) {
    pybind11::class_<BaseDetector, std::shared_ptr<BaseDetector>>(m, "BaseDetector");
}

void bind_content_detector(pybind11::module_ &m) {
    pybind11::class_<ContentDetector, BaseDetector, std::shared_ptr<ContentDetector>>(m, "ContentDetector")
        .def_static("initialize_detector", &ContentDetector::initialize_detector,
            pybind11::arg("threshold") = 27.0f, pybind11::arg("min_scene_len") = 15);
}

void bind_adaptive_detector(pybind11::module_ &m) {
    pybind11::class_<AdaptiveDetector, BaseDetector, std::shared_ptr<AdaptiveDetector>>(m, "AdaptiveDetector")
        .def_static("initialize_detector", &AdaptiveDetector::initialize_detector,
            pybind11::arg("adaptive_threshold") = 3.0f, pybind11::arg("min_scene_len") = 15,
            pybind11::arg("window_width") = 2, pybind11::arg("min_content_val") = 15.0f);
}

void bind_hash_detector(pybind11::module_ &m) {
    pybind11::class_<HashDetector, BaseDetector, std::shared_ptr<HashDetector>>(m, "HashDetector")
        .def_static("initialize_detector", &HashDetector::initialize_detector,
            pybind11::arg("threshold") = 0.395f, pybind11::arg("min_scene_len") = 15,
            pybind11::arg("dct_size") = 16, pybind11::arg("lowpass") = 2);
}

void bind_histogram_detector(pybind11::module_ &m) {
    pybind11::class_<HistogramDetector, BaseDetector, std::shared_ptr<HistogramDetector>>(m, "HistogramDetector")
        .def_static("initialize_detector", &HistogramDetector::initialize_detector,
            pybind11::arg("threshold") = 0.05f, pybind11::arg("min_scene_len") = 15,
            pybind11::arg("bins") = 256);
}

void bind_threshold_detector(pybind11::module_ &m) {
    pybind11::class_<ThresholdDetector, BaseDetector, std::shared_ptr<ThresholdDetector>>(m, "ThresholdDetector")
        .def_static("initialize_detector", &ThresholdDetector::initialize_detector,
            pybind11::arg("threshold") = 12.0f, pybind11::arg("min_scene_len") = 15,
            pybind11::arg("fade_bias") = 0.0f);
}