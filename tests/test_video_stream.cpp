#include <catch2/catch_test_macros.hpp>
#include "video_stream.hpp"

using namespace scene_manager;

TEST_CASE("video_stream", "[]") {
    REQUIRE(compute_downscale_factor(128) == 1);
}