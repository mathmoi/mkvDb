#include "mkvdb/btree/Node.hpp"

#include "mkvdb/common/Types.hpp"

#include "../RandomBlob.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace mkvdb;
using namespace mkvdb::tests;
using namespace mkvdb::btree;

TEST_CASE(
  "Node::InitializeNewNode After a call to initialize a new node, the size and byte_size are zero.")
{
    const common::FileOffset expected_size      = 0;
    const common::FileOffset expected_byte_size = 0;
    const pager::Page::PageIndex page_index     = 1;
    const pager::Page::PageSize page_size       = 512;

    pager::Page page(page_index, page_size);
    Node node(page);

    node.InitializeNewNode();
    auto actual_size      = node.size();
    auto actual_byte_size = node.byte_size();

    REQUIRE(expected_size == actual_size);
    REQUIRE(expected_byte_size == actual_byte_size);
}

// TODO : Continue this test
// TEST_CASE("Node::Insert After inserting a key/value pair, the size and byte_size are updated.")
// {
//     const common::FileOffset expected_size      = 1;
//     const common::FileOffset expected_byte_size = 2;
//     const pager::Page::PageIndex page_index     = 1;
//     const pager::Page::PageSize page_size       = 512;
//     const std::size_t key_size                  = 4;
//     const std::size_t value_size                = 128;

//     RandomBlob key(key_size);
//     RandomBlob value(value_size);
//     pager::Page page(page_index, page_size);
//     Node node(page);
//     node.InitializeNewNode();

//     node.Insert(key.data(), value.data());
//     auto actual_size      = node.size();
//     auto actual_byte_size = node.byte_size();

//     REQUIRE(expected_size == actual_size);
//     REQUIRE(expected_byte_size == actual_byte_size);
// }
