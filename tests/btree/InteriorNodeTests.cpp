#include "mkvdb/btree/InteriorNode.hpp"

#include "mkvdb/common/MkvDBException.hpp"

#include "mkvdb/pager/Header.hpp"

#include <catch2/catch_test_macros.hpp>

#include "builders/InteriorNodeBuilder.hpp"

using namespace mkvdb;
using namespace mkvdb::tests::btree::builders;
using namespace mkvdb::tests::utils;

TEST_CASE("InteriorNode::available_space Empty node, first page, correct value returned")
{
    const pager::Page::PageSize page_size        = 512;
    const pager::Page::PageSize node_header_size = 2 + 4;
    const pager::Page::PageSize expected =
      page_size - mkvdb::pager::Header::HEADER_SIZE - node_header_size;

    InteriorNodeBuilder builder;
    auto sut    = builder.WithPageIndex(0).WithPageSize(page_size).Build();

    auto result = sut.available_space();

    REQUIRE(expected == result);
}

TEST_CASE("InteriorNode::available_space Empty node, second page, correct value returned")
{
    const pager::Page::PageSize page_size        = 512;
    const pager::Page::PageSize node_header_size = 2 + 4;
    const pager::Page::PageSize expected         = page_size - node_header_size;

    InteriorNodeBuilder builder;
    auto sut    = builder.WithPageIndex(1).WithPageSize(page_size).Build();

    auto result = sut.available_space();

    REQUIRE(expected == result);
}

TEST_CASE(
  "InteriorNode::available_space node with one item inserted, correct value returned")
{
    const common::KeyValueSize key_size          = 4;
    const common::KeyValueSize value_size        = 4;
    const pager::Page::PageSize page_size        = 512;
    const pager::Page::PageSize node_header_size = 2 + 4;
    const pager::Page::PageSize expected = page_size - mkvdb::pager::Header::HEADER_SIZE -
                                           node_header_size - key_size - value_size;

    InteriorNodeBuilder builder;
    auto sut = builder.WithKeySize(key_size)
                 .WithValueSize(value_size)
                 .WithPageIndex(0)
                 .WithPageSize(page_size)
                 .WithItemsCount(1)
                 .Build();

    auto result = sut.available_space();

    REQUIRE(expected == result);
}

TEST_CASE("InteriorNode::Insert one items, the number of items is one")
{
    InteriorNodeBuilder builder;
    auto sut = builder.WithItemsCount(1).Build();

    REQUIRE(1 == sut.items_count());
}

TEST_CASE("InteriorNode::Insert two items, the number of items is two")
{
    InteriorNodeBuilder builder;
    auto sut = builder.WithItemsCount(2).Build();

    REQUIRE(2 == sut.items_count());
}

TEST_CASE(
  "InteriorNode::Insert Try to insert two items with same key, an exception is thrown")
{
    const common::KeyValueSize key_size   = 4;
    const common::KeyValueSize value_size = 4;
    const RandomBlob key_blob(key_size);
    const RandomBlob value_blob(value_size);

    InteriorNodeBuilder builder;
    auto sut = builder.WithKeySize(key_size).WithValueSize(value_size).Build();

    sut.Insert(key_blob.data(), value_blob.data());

    REQUIRE_THROWS_AS(sut.Insert(key_blob.data(), value_blob.data()),
                      mkvdb::common::MkvDBException);
}

// TODO : tester InteriorNode::Search

// InteriorNode::Search When the node contains a single key, if we search that key, we get
// the left most pointer.

// InteriorNode::Search When the node contains a single key, if we search a key smaller
// then the key we search, we get the left most pointer.

// InteriorNode::Search When the node contains a single key, if we search a key greater
// then then the key we search, we get the right most pointer.

// InteriorNode::search When the node contains multiple keys, if we search a key between
// two keys, the pointer associated with the larger key is returned.

// TODO : We'll need an At method to read a specific key/value (to split a node)
// TODO : During insert in an interior node, we need to update the righ-most-pointer.
// TODO : Think about insertions, can they be done "normaly, is there a single value
// (pointer) during insertions into an interior node?
