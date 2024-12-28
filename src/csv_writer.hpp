#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include "frame_timecode_pair.hpp"

#include <string>
#include <vector>
#include <filesystem>

template <typename T> struct WithError;

class CSVWriter {
    public:
        CSVWriter(const std::filesystem::path& output_dir);
        WithError<void> write_scenes_to_csv(const std::string& filename,
            const std::vector<FrameTimeCodePair>& scene_list) const;

    private:
        const std::filesystem::path output_dir_;
};

#endif