#include "image_extractor.hpp"
#include "error.hpp"
#include "frame_timecode.hpp"
#include "video_stream.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <numeric>
#include <fmt/core.h>

ImageExtractor::ImageExtractor(const std::filesystem::path& output_dir) : output_dir_{output_dir} {}

WithError<void> ImageExtractor::save_images(VideoStream& video,
                                            const std::filesystem::path& input_path,
                                            const std::vector<FrameTimeCodePair>& scene_list) const {
    /* get selected frames comparable to PySceneDetect */
    std::vector<SceneFrameIndex> scene_frame_list;
    for (size_t scene_i = 0; scene_i < scene_list.size(); scene_i++) {
        const FrameTimeCode start = std::get<0>(scene_list[scene_i]);
        const FrameTimeCode end = std::get<1>(scene_list[scene_i]);
        const std::vector<int32_t> frame_inds = _get_frame_inds(start, end);
        for (size_t frame_j = 0; frame_j < frame_inds.size(); frame_j++) {
            scene_frame_list.push_back(SceneFrameIndex { 
                static_cast<int32_t>(scene_i),
                static_cast<int32_t>(frame_j),
                frame_inds[frame_j]
            });
        }
    }

    /* save frames based on frame indices */
    const std::string output_dir = output_dir_.string();
    const std::string filename = input_path.stem().string();

    for (size_t sf_i = 0; sf_i < scene_frame_list.size(); sf_i++) {
        const SceneFrameIndex scene_frame_index = scene_frame_list[sf_i];
        const int32_t scene_ind = scene_frame_index.scene_ind;
        const int32_t frame_ind_in_scene = scene_frame_index.frame_ind_in_scene;
        const int32_t frame_ind_in_video = scene_frame_index.frame_ind_in_video; 

        WithError<void> err = video.seek(frame_ind_in_video);
        if (err.has_error()) {
            return err;
        }

        cv::Mat frame;
        if(!video.get_cap().read(frame)) {
            break;
        }

        const std::string output_path = fmt::format("{}/{}-scene-{:03d}-{:02d}.jpg", output_dir, filename, scene_ind, frame_ind_in_scene);
        if(!cv::imwrite(output_path, frame)) {
            const std::string error_msg = "Failed to save the frame to " + output_path;
            return WithError<void> { Error(ErrorCode::FailedToOpenFile, error_msg) };
        }
    }

    return WithError<void> { Error(ErrorCode::Success, "") };
}

std::vector<int32_t> ImageExtractor::_get_frame_inds(const FrameTimeCode& start, 
                                                     const FrameTimeCode& end) const {
    std::vector<StartEndSplitIndex> splits = _construct_splits(start, end);
    std::vector<int32_t> frame_inds;
    const int32_t split_size = splits.size();
    for (int j = 0; j < split_size; j++) {
        int32_t frame_ind;
        if ((j > 0 && j < image_num_ - 1) || image_num_ == 1) {
            frame_ind = std::round((splits[j].start + splits[j].end) / 2.0);
        } else if (j == 0) {
            frame_ind = std::min(splits[j].start + frame_margin_, splits[j].end);
        } else {
            frame_ind = std::max(splits[j].end - frame_margin_, splits[j].start);
        }
        frame_inds.push_back(frame_ind);
    }

    return frame_inds;
}

std::vector<StartEndSplitIndex> ImageExtractor::_construct_splits(const FrameTimeCode& start,
                                                                  const FrameTimeCode& end) const {
    std::vector<StartEndSplitIndex> result;

    int32_t start_ind = start.get_frame_num();
    const int32_t end_ind = end.get_frame_num();

    const int32_t total_size = std::max(1, end_ind - start_ind);
    
    if (image_num_ <= total_size) {
        const int32_t base_size = total_size / image_num_;
        const int32_t remainder = total_size % image_num_;

        for (int32_t i = 0; i < image_num_; i++) {
            const int32_t split_size = base_size + (i < remainder ? 1 : 0);
            result.push_back(StartEndSplitIndex { start_ind, start_ind + split_size - 1 });
            start_ind += split_size;
        }
    } else {
        /* save all of the frames in the scenes if image_num_ is larger than #frames in the scene. */
        for (int32_t i = 0; i <= total_size; i++) {
            result.push_back(StartEndSplitIndex { start_ind + i, start_ind + i + 1 });
        }
    }

    return result;
}