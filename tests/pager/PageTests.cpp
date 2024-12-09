#include "mkvdb/pager/Page.hpp"

#include "catch2/generators/catch_generators.hpp"

#include <catch2/catch_test_macros.hpp>

#include <limits>

using namespace mkvdb::pager;
using namespace Catch::Generators;

TEST_CASE("Page::index() returns the same index passed in the constructor")
{
    Page::PageIndex index = GENERATE(static_cast<Page::PageIndex>(0),
                                     static_cast<Page::PageIndex>(1),
                                     std::numeric_limits<Page::PageIndex>::max());
    Page::PageSize size   = 512;
    Page sut(index, size);

    auto result = sut.index();

    REQUIRE(index == result);
}

TEST_CASE("Page::size() returns the same size passed in the constructor")
{
    Page::PageIndex index = 42;
    Page::PageSize size   = GENERATE(512, 1024, 65536);
    Page sut(index, size);

    auto result = sut.size();

    REQUIRE(size == result);
}

TEST_CASE("Page::buffer_view() returns a ByteSpan of the expected size")
{
    Page::PageIndex index = 42;
    Page::PageSize size   = GENERATE(512, 1024, 65536);
    Page sut(index, size);

    Page::PageSize result = sut.buffer_view().const_span().size();

    REQUIRE(size == result);
}
