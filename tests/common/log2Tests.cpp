#include "mkvdb/common/log2.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

using namespace mkvdb::common;

TEST_CASE("log2 is returning expected results")
{
    auto [mask, expected] = GENERATE(std::make_tuple(0x00000001u, 0u),
                                     std::make_tuple(0x00000010u, 4u),
                                     std::make_tuple(0x80000000u, 31u),
                                     std::make_tuple(0xffffffffu, 31u));

    auto result = log2(mask);

    REQUIRE(expected == result);
}