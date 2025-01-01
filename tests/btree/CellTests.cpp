#include "mkvdb/btree/Cell.hpp"

#include "mkvdb/common/Types.hpp"

#include "../RandomBlob.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>

using namespace mkvdb;
using namespace mkvdb::tests;
using namespace mkvdb::btree;

TEST_CASE(
  "Cell::CalculateRequiredSize The size of a cell is the sum of the key size, value size and the size of the size fields.")
{
    const std::uint64_t key_size   = GENERATE(1, 42, 1024);
    const std::uint64_t value_size = GENERATE(1, 42, 1024);

    auto actual = Cell::CalculateRequiredSize(key_size, value_size);

    REQUIRE(sizeof(common::ValueSize) * 2 + key_size + value_size == actual);
}

TEST_CASE("Cell::key After creating a cell we can read the key back")
{
    RandomBlob key(12);
    RandomBlob value(42);
    auto required_size = Cell::CalculateRequiredSize(key.size(), value.size());
    std::vector<std::byte> buffer(required_size);
    Cell sut(buffer, key, value);

    auto actual = sut.key();

    REQUIRE_THAT(key.data(), Catch::Matchers::RangeEquals(actual));
}

TEST_CASE("Cell::Value After creating a cell we can read the value back")
{
    RandomBlob key(12);
    RandomBlob value(42);
    auto required_size = Cell::CalculateRequiredSize(key.size(), value.size());
    std::vector<std::byte> buffer(required_size);
    Cell sut(buffer, key, value);

    auto actual = sut.value();

    REQUIRE_THAT(value.data(), Catch::Matchers::RangeEquals(actual));
}

TEST_CASE(
  "Cell::Cell After creating a cell, we can reuse the buffer to recreate the cell and read the original data back.")
{
    RandomBlob key(12);
    RandomBlob value(42);
    auto required_size = Cell::CalculateRequiredSize(key.size(), value.size());
    std::vector<std::byte> buffer(required_size);

    Cell sut(buffer, key, value);
    Cell sut2(buffer);
    auto actual_key   = sut2.key();
    auto actual_value = sut2.value();

    REQUIRE_THAT(key.data(), Catch::Matchers::RangeEquals(actual_key));
    REQUIRE_THAT(value.data(), Catch::Matchers::RangeEquals(actual_value));
}
