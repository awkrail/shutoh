#include <catch2/catch_test_macros.hpp>
#include "scene_manager.hpp"

using namespace scene_manager;

TEST_CASE("compute_downscale_factor", "[compute_downscale_factor]") {
    REQUIRE(compute_downscale_factor(128) == 1);
    REQUIRE(compute_downscale_factor(255) == 1);
    REQUIRE(compute_downscale_factor(512) == 2);
}