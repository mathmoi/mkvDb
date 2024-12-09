#ifndef MKVDB_BTREE_LEAF_NODE_HPP_
#define MKVDB_BTREE_LEAF_NODE_HPP_

#include "mkvdb/pager/Page.hpp"

#include "Node.hpp"

namespace mkvdb::btree
{
    // Represents an leaf node of a btree. Leaf nodes contains keys and values.
    class LeafNode : public Node
    {
    public:
        // Constructor.
        LeafNode(const BTree& btree, std::shared_ptr<pager::Page> page, bool initialize);

        // Destructor.
        virtual ~LeafNode() {}

        // Search an item in the node with a given key. If the item is found, the methods
        // returns true after the value is copied in buffer. The buffer must be big enough
        // to accept the whole value. If the key is not found, the methods returns false
        // and the buffer is unchanged.
        bool Search(common::ConstByteSpan key, common::ByteSpan buffer);
    };

} // namespace mkvdb::btree

#endif // MKVDB_BTREE_LEAF_NODE_HPP_