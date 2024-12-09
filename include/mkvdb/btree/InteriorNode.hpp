#ifndef MKVDB_BTREE_INTERIOR_NODE_HPP_
#define MKVDB_BTREE_INTERIOR_NODE_HPP_

#include "mkvdb/common/Serialization.hpp"
#include "mkvdb/common/Types.hpp"

#include "mkvdb/pager/Page.hpp"

#include "Node.hpp"

namespace mkvdb::btree
{
    // Represents an interior node of a btree. Interior nodes contains keys and pointer to
    // other interior or leaf nodes.
    //
    // This class add the following values to the node header
    //
    //    Offset Size Description
    //    ------ ---- --------------------------------------------------------------------
    //     0      4   Right most pointer. Pointer to the child node containings keys that
    //                are greater than all the keys in the current node.
    class InteriorNode : public Node
    {
    public:
        // Constructor.
        InteriorNode(const BTree& btree,
                     std::shared_ptr<pager::Page> page,
                     bool initialize);

        // Destructor.
        virtual ~InteriorNode() {}

        // Insert a new items in the node
        inline void Insert(common::ConstByteSpan key, pager::Page::PageIndex index);
        using Node::Insert;

        // Returns a pointer to the child node where the given key can be found if it is
        // presents in the tree.
        pager::Page::PageIndex Search(common::ConstByteSpan key);

    protected:
        // Returns the size of the page header.
        common::FileSize header_size() const override;

    private:
        static const common::FileSize RIGHT_MOST_POINTER_SIZE     = 4;

        static const common::FileOffset RIGHT_MOST_POINTER_OFFSET = 0;

        inline common::ByteSpan right_most_pointer_span() const;
        inline pager::Page::PageIndex right_most_pointer() const;
    };

    void InteriorNode::Insert(common::ConstByteSpan key, pager::Page::PageIndex index)
    {
        std::array<std::byte, sizeof(pager::Page::PageIndex)> buffer;
        common::Serialize(index, buffer);
        Node::Insert(key, buffer);
    }

    common::ByteSpan InteriorNode::right_most_pointer_span() const
    {
        return page()->content().subspan(Node::header_size() + RIGHT_MOST_POINTER_OFFSET,
                                         RIGHT_MOST_POINTER_SIZE);
    }

    pager::Page::PageIndex InteriorNode::right_most_pointer() const
    {
        return common::Deserialize<pager::Page::PageIndex>(right_most_pointer_span());
    }

} // namespace mkvdb::btree

#endif // MKVDB_BTREE_INTERIOR_NODE_HPP_