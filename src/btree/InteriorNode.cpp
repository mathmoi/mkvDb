#include "mkvdb/btree/InteriorNode.hpp"

#include "mkvdb/common/Serialization.hpp"

#include "mkvdb/pager/Page.hpp"

namespace mkvdb::btree
{
    InteriorNode::InteriorNode(const BTree& btree,
                               std::shared_ptr<pager::Page> page,
                               bool initialize)
    : Node(btree, page, sizeof(pager::Page::PageIndex), initialize)
    {
    }

    common::FileSize InteriorNode::header_size() const
    {
        return Node::header_size() + RIGHT_MOST_POINTER_SIZE;
    }

    pager::Page::PageIndex InteriorNode::Search(common::ConstByteSpan key)
    {
        assert(0 < items_count()); // A valid interior nodes has at least one key and two
                                   // children. We should not search into an, invalid,
                                   // empty node.

        auto [index, _] = SearchIndex(key);

        if(index < items_count())
        {
            auto value = GetItemValue(index);
            return common::Deserialize<pager::Page::PageIndex>(value);
        }

        // If the index is equal to the number of elements in the node, this means the key
        // is greater than all the keys in the node. In this situation de
        // left-most-pointer is returned.
        return right_most_pointer();
    }

} // namespace mkvdb::btree