#include "mkvdb/btree/NodeHeader.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

using namespace mkvdb;
using namespace mkvdb::btree;

TEST_CASE("NodeHeader::size Returns the size previously set")
{
    NodeHeader::NodeSize node_size =
      GENERATE(0,                                         // Minimum
               std::numeric_limits<std::uint16_t>::max(), // Maximum
               take(10, random(UINT16_C(0), UINT16_MAX))  // Random values
      );

    std::array<std::byte, NodeHeader::HEADER_SIZE> buffer;
    NodeHeader sut(buffer);

    sut.size(node_size);
    auto actual = sut.size();

    REQUIRE(node_size == actual);
}

TEST_CASE("NodeHeader::byte_size Returns the size previously set")
{
    NodeHeader::ByteSize byte_size = GENERATE(
      INT64_C(0),                                // Minimum
      std::numeric_limits<std::uint32_t>::max(), // Maximum
      take(10,
           random(static_cast<int64_t>(0),
                  static_cast<int64_t>(std::numeric_limits<std::uint32_t>::max()))) // Random values
    );

    std::array<std::byte, NodeHeader::HEADER_SIZE> buffer;
    NodeHeader sut(buffer);

    sut.byte_size(byte_size);
    auto actual = sut.byte_size();

    REQUIRE(byte_size == actual);
}

TEST_CASE("NodeHeader::unallocated_space Returns the size previously set")
{
    NodeHeader::ByteSize unallocated_space = GENERATE(
      INT64_C(0),                                // Minimum
      std::numeric_limits<std::uint16_t>::max(), // Maximum
      take(10,
           random(static_cast<int64_t>(0),
                  static_cast<int64_t>(std::numeric_limits<std::uint16_t>::max()))) // Random values
    );

    std::array<std::byte, NodeHeader::HEADER_SIZE> buffer;
    NodeHeader sut(buffer);

    sut.unallocated_space(unallocated_space);
    auto actual = sut.unallocated_space();

    REQUIRE(unallocated_space == actual);
}