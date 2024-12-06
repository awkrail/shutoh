#include <catch2/catch_test_macros.hpp>

#include "video_frame.hpp"
#include "content_detector.hpp"

using namespace content_detector;

TEST_CASE("compute frame score", "") {
    REQUIRE(compute_downscale_factor(128) == 1);
    REQUIRE(compute_downscale_factor(255) == 1);
}