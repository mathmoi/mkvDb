#include "mkvdb/btree/LeafNode.hpp"

namespace mkvdb::btree
{
    LeafNode::LeafNode(const BTree& btree,
                       std::shared_ptr<pager::Page> page,
                       bool initialize)
    : Node(btree, page, btree.value_size(), initialize)
    {
    }

    bool LeafNode::Search(common::ConstByteSpan key, common::ByteSpan buffer)
    {
        assert(value_size() <= buffer.size_bytes());

        auto search_result = SearchIndex(key);
        if(search_result.second)
        {
            auto value = GetItemValue(search_result.first);
            std::copy(value.begin(), value.end(), buffer.begin());
        }

        return search_result.second;
    }

} // namespace mkvdb::btree