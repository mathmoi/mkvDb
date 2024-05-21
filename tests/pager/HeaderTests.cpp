#include "mkvdb/pager/Header.hpp"

#include "mkvdb/common/Serialization.hpp"
#include "mkvdb/common/Types.hpp"

#include "mkvdb/fs/memory/MemoryFile.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <cstddef>
#include <tuple>

using namespace mkvdb;
using namespace mkvdb::pager;

TEST_CASE("Header::ReadPageSize returns the correct value")
{
    auto [content, expected] =
      GENERATE(std::make_tuple("6d6b7644422066696c6520763100000009", 512u),
               std::make_tuple("6d6b7644422066696c652076310000000a", 1024u),
               std::make_tuple("6d6b7644422066696c652076310000000b", 2048u),
               std::make_tuple("6d6b7644422066696c652076310000000c", 4096u),
               std::make_tuple("6d6b7644422066696c652076310000000d", 8192u),
               std::make_tuple("6d6b7644422066696c652076310000000e", 16384u),
               std::make_tuple("6d6b7644422066696c652076310000000f", 32768u),
               std::make_tuple("6d6b7644422066696c6520763100000010", 65536u));
    fs::memory::MemoryFile file(common::SerializeHex(content));
    file.Open();

    auto result = Header::ReadPageSize(file);

    REQUIRE(expected == result);
}

TEST_CASE("Header::Initialize add the magic string in the header")
{
    const std::string expected("6d6b7644422066696c65207631");

    fs::memory::MemoryFile file;
    file.Open();

    Header::Initialize(file, 512);

    auto result = common::DeserializeHex(file.data().subspan(0, 13));
    REQUIRE(expected == result);
}

TEST_CASE("Header::Initialize add the correct page size value in the header")
{
    auto expected = std::byte(0x0b);

    fs::memory::MemoryFile file;
    file.Open();

    Header::Initialize(file, 2048);

    auto result = file.data()[16];
    REQUIRE(expected == result);
}

TEST_CASE("Header::Initialize the number of pages in the database is one")
{
    Page::PageIndex expected = 1;

    fs::memory::MemoryFile file;
    file.Open();

    Header::Initialize(file, 2048);

    auto result = common::Deserialize<std::uint32_t>(file.data().subspan(17, 4));
    REQUIRE(expected == result);
}

TEST_CASE("Header::pages_count(...) correctly changes the page count")
{
    auto page = std::make_shared<Page>(0, 512);
    common::SerializeHex("6d6b7644422066696c652076310000000989c9c0f6", page->data());
    Header sut(page);
    Page::PageIndex new_count = 0x0910bc1e;

    sut.pages_count(new_count);

    REQUIRE(new_count == sut.pages_count());
}