#include "mkvdb/pager/Page.hpp"

#include "mkvdb/pager/Header.hpp"

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

TEST_CASE("Page::data() returns a ByteSpan of the expected size")
{
    Page::PageIndex index = 42;
    Page::PageSize size   = GENERATE(512, 1024, 65536);
    Page sut(index, size);

    Page::PageSize result = sut.data().size();

    REQUIRE(size == result);
}

TEST_CASE(
  "Page::content() For the index zero content returns a ByteSpan with the header size removed")
{
    Page::PageIndex index   = 0;
    Page::PageSize size     = GENERATE(512, 1024, 65536);
    Page::PageSize expected = size - Header::HEADER_SIZE;
    Page sut(index, size);

    Page::PageSize result = sut.content().size();

    REQUIRE(expected == result);
}

TEST_CASE(
  "Page::content() For the any index except zero content returns the same as data")
{
    Page::PageIndex index = GENERATE(1, 42, std::numeric_limits<Page::PageIndex>::max());
    Page::PageSize size   = GENERATE(512, 1024, 65536);
    Page sut(index, size);
    auto expected = sut.data();

    auto result = sut.content();

    REQUIRE(expected.data() == result.data());
    REQUIRE(expected.size() == result.size());
}

TEST_CASE("Page::is_modified() is false by default")
{
    auto expected = false;

    Page sut(0, 512);

    auto result = sut.is_modified();

    REQUIRE(expected == result);
}
