#include "mkvdb/btree/LeafNode.hpp"

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_range_equals.hpp"

#include "../utils/RandomBlob.hpp"
#include "builders/LeafNodeBuilder.hpp"

using namespace mkvdb::tests::utils;
using namespace mkvdb::tests::btree::builders;

TEST_CASE("LeafNode::Search When the node is empty the search fails to find an item")
{
    const mkvdb::common::KeyValueSize key_size   = 4;
    const mkvdb::common::KeyValueSize value_size = 4;
    const RandomBlob key(key_size);

    std::vector<std::byte> buffer(value_size);
    LeafNodeBuilder builder;
    auto sut    = builder.WithKeySize(key_size).WithValueSize(value_size).Build();

    auto result = sut.Search(key, buffer);

    REQUIRE_FALSE(result);
}

TEST_CASE(
  "LeafNode::Search When the nodes contains a couples of items, but not the one we search for, the search fails")
{
    const mkvdb::common::KeyValueSize key_size   = 4;
    const mkvdb::common::KeyValueSize value_size = 4;
    const RandomBlob key(key_size);

    std::vector<std::byte> buffer(value_size);
    LeafNodeBuilder builder;
    auto sut =
      builder.WithKeySize(key_size).WithValueSize(value_size).WithItemsCount(10).Build();

    auto result = sut.Search(key, buffer);

    REQUIRE_FALSE(result);
}

TEST_CASE("LeafNode::Search When the node contains one item we can read back that item")
{
    const mkvdb::common::KeyValueSize key_size   = 4;
    const mkvdb::common::KeyValueSize value_size = 4;
    const RandomBlob key(key_size);
    const RandomBlob value(value_size);

    std::vector<std::byte> buffer(value_size);
    LeafNodeBuilder builder;
    auto sut = builder.WithKeySize(key_size).WithValueSize(value_size).Build();
    sut.Insert(key, value);

    auto result = sut.Search(key, buffer);

    REQUIRE(result);
    REQUIRE_THAT(value, Catch::Matchers::RangeEquals(buffer));
}

TEST_CASE(
  "LeafNode::Search When the nodes contains a couples of items, including the one we search for, we can read the item's value correctly")
{
    const mkvdb::common::KeyValueSize key_size   = 4;
    const mkvdb::common::KeyValueSize value_size = 4;
    const RandomBlob key(key_size);
    const RandomBlob value(value_size);

    std::vector<std::byte> buffer(value_size);
    LeafNodeBuilder builder;
    auto sut =
      builder.WithKeySize(key_size).WithValueSize(value_size).WithItemsCount(10).Build();
    sut.Insert(key, value);

    auto result = sut.Search(key, buffer);

    REQUIRE(result);
    REQUIRE_THAT(value, Catch::Matchers::RangeEquals(buffer));
}
