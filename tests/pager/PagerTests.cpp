#include "mkvdb/pager/Pager.hpp"

#include "mkvdb/fs/memory/MemoryFile.hpp"

#include "mkvdb/pager/Header.hpp"

#include "../utils/RandomBlob.hpp"
#include "catch2/matchers/catch_matchers_range_equals.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <algorithm>

using namespace mkvdb::fs::memory;
using namespace mkvdb::pager;
using namespace mkvdb::tests;
using namespace mkvdb::tests::utils;

TEST_CASE("Pager::GetNewPage returns a new page with the correct index")
{
    mkvdb::fs::memory::MemoryFile file;
    file.Open();
    Header::Initialize(file, 512);
    Pager sut(file);

    auto page        = sut.GetNewPage();
    auto second_page = sut.GetNewPage();

    REQUIRE(1 == page->index());
    REQUIRE(2 == second_page->index());
}

TEST_CASE("Pager::GetNewPage returns a new page with the correct size")
{
    const Page::PageSize page_size = GENERATE(512, 1024, 65536);

    mkvdb::fs::memory::MemoryFile file;
    file.Open();
    Header::Initialize(file, page_size);
    Pager sut(file);

    auto page = sut.GetNewPage();

    REQUIRE(page_size == page->size());
}

TEST_CASE("Pager::GetPage returns the correct page index")
{
    const Page::PageSize page_size   = 512;
    const Page::PageIndex index      = GENERATE(0, 1, 2);
    const Page::PageIndex page_count = 3;

    RandomBlob blob(page_size * page_count);
    mkvdb::fs::memory::MemoryFile file;
    file.Open();
    file.Write(blob.data(), 0);
    Header::Initialize(file, page_size);
    Pager sut(file);

    auto page = sut.GetPage(index);

    REQUIRE(index == page->index());
}

TEST_CASE("Pager::GetPage returns the correct page content")
{
    const Page::PageSize page_size   = 512;
    const Page::PageIndex index      = GENERATE(1, 2);
    const Page::PageIndex page_count = 3;

    RandomBlob blob(page_size * page_count);
    mkvdb::fs::memory::MemoryFile file;
    file.Open();
    file.Write(blob.data(), 0);
    Header::Initialize(file, page_size);
    Pager sut(file);

    auto page = sut.GetPage(index);

    REQUIRE_THAT(
      page->data(),
      Catch::Matchers::RangeEquals(blob.data().subspan(page_size * index, page_size)));
}

TEST_CASE("Pager::WriteModifiedPages write the pages marked as modified")
{
    const Page::PageSize page_size              = 512;
    const Page::PageIndex page_count            = 3;
    const Page::PageIndex index_modified_page   = 1;
    const Page::PageIndex index_unmodified_page = 2;

    RandomBlob original_content(page_size * page_count);
    RandomBlob modified_page_content(page_size);
    mkvdb::fs::memory::MemoryFile file;
    file.Open();
    file.Write(original_content.data(), 0);
    Header::Initialize(file, page_size);
    Pager sut(file);

    auto page = sut.GetPage(index_modified_page);
    std::copy(modified_page_content.data().begin(),
              modified_page_content.data().end(),
              page->data().begin());
    page->MarkAsModified();
    sut.WriteModifiedPages();

    REQUIRE_THAT(file.data().subspan(index_modified_page * page_size, page_size),
                 Catch::Matchers::RangeEquals(modified_page_content.data()));
    REQUIRE_THAT(
      file.data().subspan(index_unmodified_page * page_size, page_size),
      Catch::Matchers::RangeEquals(
        original_content.data().subspan(index_unmodified_page * page_size, page_size)));
}

TEST_CASE("Pager::WriteModifiedPages pages not marked as modified are not written")
{
    const Page::PageSize page_size   = 512;
    const Page::PageIndex page_count = 3;
    const Page::PageIndex index      = 1;

    RandomBlob original_content(page_size * page_count);
    RandomBlob modified_page_content(page_size);
    mkvdb::fs::memory::MemoryFile file;
    file.Open();
    file.Write(original_content.data(), 0);
    Header::Initialize(file, page_size);
    Pager sut(file);

    auto page = sut.GetPage(index);
    std::copy(modified_page_content.data().begin(),
              modified_page_content.data().end(),
              page->data().begin());
    sut.WriteModifiedPages();

    REQUIRE_THAT(file.data().subspan(index * page_size, page_size),
                 Catch::Matchers::RangeEquals(
                   original_content.data().subspan(index * page_size, page_size)));
}