#include "mkvdb/btree/SlotArray.hpp"

#include "mkvdb/common/Types.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

using namespace mkvdb;
using namespace mkvdb::btree;

TEST_CASE(
  "SlotArray::Insert After inserting new offsets the size of the node is update accordingly.")
{
    const std::uint64_t buffer_size     = 512;
    const std::uint16_t insert_position = 0;
    const std::uint16_t offset_value    = 42;
    const std::uint16_t offset_number   = GENERATE(1, 2, 3);

    std::array<std::byte, buffer_size> buffer;
    NodeHeader header(common::ByteSpan(buffer).subspan(0, NodeHeader::HEADER_SIZE));
    header.size(0);
    header.unallocated_space(buffer_size - NodeHeader::HEADER_SIZE);
    SlotArray sut(header, buffer);

    for(std::uint16_t i = 0; i < offset_number; ++i)
    {
        sut.Insert(insert_position, offset_value);
    }
    auto actual_size = header.size();

    REQUIRE(offset_number == actual_size);
}

TEST_CASE("SlotArray::At After inserting an offset it can be read back.")
{
    const std::uint64_t buffer_size     = 512;
    const std::uint16_t insert_position = 0;
    const std::uint16_t offset_value    = 42;

    std::array<std::byte, buffer_size> buffer;
    NodeHeader header(common::ByteSpan(buffer).subspan(0, NodeHeader::HEADER_SIZE));
    header.size(0);
    header.unallocated_space(buffer_size - NodeHeader::HEADER_SIZE);
    SlotArray sut(header, buffer);

    sut.Insert(insert_position, offset_value);
    auto actual = sut.At(insert_position);

    REQUIRE(offset_value == actual);
}

TEST_CASE("SlotArray::At After inserting in the middle of the array the offset can be read back.")
{
    const std::uint64_t buffer_size     = 512;
    const std::uint16_t insert_position = 1;
    const std::uint16_t offset_value    = 42;

    std::array<std::byte, buffer_size> buffer;
    NodeHeader header(common::ByteSpan(buffer).subspan(0, NodeHeader::HEADER_SIZE));
    header.size(0);
    header.unallocated_space(buffer_size - NodeHeader::HEADER_SIZE);
    SlotArray sut(header, buffer);

    sut.Insert(0, 0);
    sut.Insert(0, 0);
    sut.Insert(insert_position, offset_value);
    auto actual = sut.At(insert_position);

    REQUIRE(offset_value == actual);
}

TEST_CASE("SlotArray::Erase After erasing the only offset the size of the node is updated.")
{
    const std::uint64_t buffer_size     = 512;
    const std::uint16_t insert_position = 0;
    const std::uint16_t offset_value    = 42;

    std::array<std::byte, buffer_size> buffer;
    NodeHeader header(common::ByteSpan(buffer).subspan(0, NodeHeader::HEADER_SIZE));
    header.size(0);
    header.unallocated_space(buffer_size - NodeHeader::HEADER_SIZE);
    SlotArray sut(header, buffer);

    sut.Insert(insert_position, offset_value);
    sut.Erase(insert_position);
    auto actual_size = header.size();

    REQUIRE(0 == actual_size);
}

TEST_CASE(
  "SlotArray::Erase After erasing the second offset, the third offset can be red at the second position.")
{
    const std::uint64_t buffer_size     = 512;
    const std::uint16_t insert_position = 2;
    const std::uint16_t erase_position  = 1;
    const std::uint16_t offset_value    = 42;

    std::array<std::byte, buffer_size> buffer;
    NodeHeader header(common::ByteSpan(buffer).subspan(0, NodeHeader::HEADER_SIZE));
    header.size(0);
    header.unallocated_space(buffer_size - NodeHeader::HEADER_SIZE);
    SlotArray sut(header, buffer);

    sut.Insert(0, 0);
    sut.Insert(0, 0);
    sut.Insert(insert_position, offset_value);

    sut.Erase(erase_position);
    auto actual = sut.At(erase_position);

    REQUIRE(offset_value == actual);
}

TEST_CASE(
  "SlotArray::Insert After inserting 1, 2 or 3 offsets, the unnalocated space is reduced accordingly")
{
    const std::uint64_t buffer_size     = 512;
    const std::uint16_t insert_position = 0;
    const std::uint16_t offset_value    = 42;
    const std::uint16_t offset_number   = GENERATE(1, 2, 3);
    const std::uint16_t offset_size     = 2;

    std::array<std::byte, buffer_size> buffer;
    NodeHeader header(common::ByteSpan(buffer).subspan(0, NodeHeader::HEADER_SIZE));
    header.size(0);
    header.unallocated_space(buffer_size - NodeHeader::HEADER_SIZE);
    SlotArray sut(header, buffer);

    for(std::uint16_t i = 0; i < offset_number; ++i)
    {
        sut.Insert(insert_position, offset_value);
    }
    auto actual = header.unallocated_space();

    REQUIRE(buffer_size - NodeHeader::HEADER_SIZE - offset_number * offset_size == actual);
}