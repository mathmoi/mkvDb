#include "mkvdb/common/Types.hpp"

#include "../RandomBlob.hpp"

#include <catch2/catch_test_macros.hpp>

#include <mkvdb/pager/BufferView.hpp>

using namespace mkvdb::common;
using namespace mkvdb::pager;
using namespace mkvdb::tests;

TEST_CASE("BufferView::is_modified returns true for a new object")
{
    const size_t BUFFER_SIZE = 128;

    std::array<std::byte, BUFFER_SIZE> buffer;
    BufferView sut(buffer);

    auto is_modified = sut.is_modified();

    REQUIRE_FALSE(is_modified);
}

TEST_CASE("BufferView::buffer() returns a byte span to to buffer")
{
    const size_t BUFFER_SIZE = 128;

    RandomBlob blob(BUFFER_SIZE);
    std::array<std::byte, BUFFER_SIZE> buffer;
    std::copy(blob.begin(), blob.end(), buffer.begin());

    BufferView sut(buffer);

    auto result = sut.buffer();

    REQUIRE(BUFFER_SIZE == result.size());
    REQUIRE(std::equal(blob.begin(), blob.end(), result.begin()));
}

TEST_CASE("BufferView::Mutate allows to modify the underlying buffer")
{
    const size_t BUFFER_SIZE   = 128;
    const size_t BYTE_INDEX    = 42;
    const std::byte TEST_VALUE = std::byte { 0x69 };

    RandomBlob blob(BUFFER_SIZE);
    std::array<std::byte, BUFFER_SIZE> buffer;
    std::copy(blob.begin(), blob.end(), buffer.begin());

    BufferView sut(buffer);

    sut.Mutate([](ByteSpan buffer) { buffer[BYTE_INDEX] = TEST_VALUE; });
    auto is_modified = sut.is_modified();

    REQUIRE(is_modified);
    REQUIRE(TEST_VALUE == buffer[BYTE_INDEX]);
}

TEST_CASE(
  "BufferView::GetSubView can be use to create a BufferView of a part of the original BufferView")
{
    const size_t BUFFER_SIZE       = 128;
    const size_t SUB_BUFFER_OFFSET = 32;
    const size_t SUB_BUFFER_SIZE   = 32;

    RandomBlob blob(BUFFER_SIZE);
    std::array<std::byte, BUFFER_SIZE> buffer;
    std::copy(blob.begin(), blob.end(), buffer.begin());

    BufferView sut(buffer);

    auto result = sut.GetSubView(SUB_BUFFER_OFFSET, SUB_BUFFER_SIZE);

    REQUIRE(SUB_BUFFER_SIZE == result.buffer().size());
    REQUIRE(std::equal(buffer.begin() + SUB_BUFFER_OFFSET,
                       buffer.begin() + SUB_BUFFER_OFFSET + SUB_BUFFER_SIZE,
                       result.buffer().begin()));
}

TEST_CASE("BufferView::Mutate will mark parents views as modified.")
{
    const size_t BUFFER_SIZE       = 128;
    const size_t SUB_BUFFER_OFFSET = 32;
    const size_t SUB_BUFFER_SIZE   = 32;
    const size_t MODIFY_INDEX      = 4;
    const std::byte NEW_VALUE      = std::byte { 0x42 };

    RandomBlob blob(BUFFER_SIZE);
    std::array<std::byte, BUFFER_SIZE> buffer;
    std::copy(blob.begin(), blob.end(), buffer.begin());

    BufferView sut(buffer);
    auto sub_view = sut.GetSubView(SUB_BUFFER_OFFSET, SUB_BUFFER_SIZE);

    sub_view.Mutate([](ByteSpan buffer) { buffer[MODIFY_INDEX] = NEW_VALUE; });

    REQUIRE(sut.is_modified());
}
