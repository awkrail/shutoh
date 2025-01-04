#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include "frame_timecode_pair.hpp"

#include <string>
#include <vector>
#include <filesystem>

template <typename T> struct WithError;

class CSVWriter {
    public:
        CSVWriter(const std::filesystem::path& output_dir, const bool no_output_file);
        WithError<void> list_scenes(const std::filesystem::path& input_path,
                                    const std::vector<FrameTimeCodePair>& scene_list) const;

    private:
        void _print_scenes(const std::vector<FrameTimeCodePair>& scene_list) const;
        WithError<void> _write_scenes_to_csv(const std::filesystem::path& input_path,
                                             const std::vector<FrameTimeCodePair>& scene_list) const;

        const std::filesystem::path output_dir_;
        const bool no_output_file_;
};

#endif