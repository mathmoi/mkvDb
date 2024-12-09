#include "mkvdb/btree/Node.hpp"

#include "mkvdb/common/MkvDBException.hpp"
#include "mkvdb/common/Types.hpp"

#include "mkvdb/pager/Header.hpp"
#include "mkvdb/pager/Page.hpp"

#include "catch2/catch_test_macros.hpp"

#include "../utils/RandomBlob.hpp"
#include "builders/NodeBuilder.hpp"

using namespace mkvdb;
using namespace mkvdb::tests::utils;
using namespace mkvdb::tests::btree::builders;

TEST_CASE("Node::Node Creates a new node with zero items")
{
    NodeBuilder builder;
    auto sut = builder.Build();

    REQUIRE(0 == sut.items_count());
}

TEST_CASE("Node::Insert one items, the number of items is one")
{
    NodeBuilder builder;
    auto sut = builder.WithItemsCount(1).Build();

    REQUIRE(1 == sut.items_count());
}

TEST_CASE("Node::Insert two items, the number of items is two")
{
    NodeBuilder builder;
    auto sut = builder.WithItemsCount(2).Build();

    REQUIRE(2 == sut.items_count());
}

TEST_CASE("Node::Insert Try to insert two items with same key, an exception is thrown")
{
    const common::KeyValueSize key_size   = 4;
    const common::KeyValueSize value_size = 4;
    const RandomBlob key_blob(key_size);
    const RandomBlob value_blob(value_size);

    NodeBuilder builder;
    auto sut = builder.WithKeySize(key_size).WithValueSize(value_size).Build();

    sut.Insert(key_blob.data(), value_blob.data());

    REQUIRE_THROWS_AS(sut.Insert(key_blob.data(), value_blob.data()),
                      mkvdb::common::MkvDBException);
}

TEST_CASE("Node::available_space Empty node, first page, correct value returned")
{
    const mkvdb::pager::Page::PageSize page_size        = 512;
    const mkvdb::pager::Page::PageSize node_header_size = 2;
    const mkvdb::pager::Page::PageSize expected =
      page_size - mkvdb::pager::Header::HEADER_SIZE - node_header_size;

    NodeBuilder builder;
    auto sut    = builder.WithPageIndex(0).WithPageSize(page_size).Build();

    auto result = sut.available_space();

    REQUIRE(expected == result);
}

TEST_CASE("Node::available_space Empty node, second page, correct value returned")
{
    const mkvdb::pager::Page::PageSize page_size        = 512;
    const mkvdb::pager::Page::PageSize node_header_size = 2;
    const mkvdb::pager::Page::PageSize expected         = page_size - node_header_size;

    NodeBuilder builder;
    auto sut    = builder.WithPageIndex(1).WithPageSize(page_size).Build();

    auto result = sut.available_space();

    REQUIRE(expected == result);
}

TEST_CASE("Node::available_space node with one item inserted, correct value returned")
{
    const mkvdb::common::KeyValueSize key_size          = 4;
    const mkvdb::common::KeyValueSize value_size        = 4;
    const mkvdb::pager::Page::PageSize page_size        = 512;
    const mkvdb::pager::Page::PageSize node_header_size = 2;
    const mkvdb::pager::Page::PageSize expected =
      page_size - mkvdb::pager::Header::HEADER_SIZE - node_header_size - key_size -
      value_size;

    NodeBuilder builder;
    auto sut = builder.WithKeySize(key_size)
                 .WithValueSize(value_size)
                 .WithPageIndex(0)
                 .WithPageSize(page_size)
                 .WithItemsCount(1)
                 .Build();

    auto result = sut.available_space();

    REQUIRE(expected == result);
}

TEST_CASE("Node::CheckForSpace When the node is empty returns true")
{
    NodeBuilder builder;
    auto sut    = builder.Build();

    auto result = sut.CheckForSpace();

    REQUIRE(result);
}

TEST_CASE("Node::CheckForSpace Only space left for one item. Returns true.")
{
    const mkvdb::pager::Page::PageSize page_size        = 512;
    const mkvdb::pager::Page::PageIndex page_index      = 2;
    const mkvdb::common::KeyValueSize key_size          = 4;
    const mkvdb::common::KeyValueSize value_size        = 4;
    const mkvdb::pager::Page::PageSize node_header_size = 2;
    const std::uint32_t items_count =
      (page_size - node_header_size) / (key_size + value_size) - 1;

    NodeBuilder builder;
    auto sut = builder.WithPageSize(page_size)
                 .WithPageIndex(page_index)
                 .WithKeySize(key_size)
                 .WithValueSize(value_size)
                 .WithItemsCount(items_count)
                 .Build();

    auto result = sut.CheckForSpace();

    REQUIRE(result);
}

TEST_CASE("Node::CheckForSpace if node is full, returns false.")
{
    const mkvdb::pager::Page::PageSize page_size        = 512;
    const mkvdb::pager::Page::PageIndex page_index      = 2;
    const mkvdb::common::KeyValueSize key_size          = 4;
    const mkvdb::common::KeyValueSize value_size        = 4;
    const mkvdb::pager::Page::PageSize node_header_size = 2;
    const std::uint32_t items_count =
      (page_size - node_header_size) / (key_size + value_size);

    NodeBuilder builder;
    auto sut = builder.WithPageSize(page_size)
                 .WithPageIndex(page_index)
                 .WithKeySize(key_size)
                 .WithValueSize(value_size)
                 .WithItemsCount(items_count)
                 .Build();

    auto result = sut.CheckForSpace();

    REQUIRE_FALSE(result);
}