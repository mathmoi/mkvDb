#ifndef MKVDB_BTREE_NODE_HPP_
#define MKVDB_BTREE_NODE_HPP_

#include "mkvdb/common/Serialization.hpp"
#include "mkvdb/common/Types.hpp"

#include "mkvdb/pager/Page.hpp"

#include "mkvdb/btree/BTree.hpp"

namespace mkvdb::btree
{
    // Forward declaration
    class BTree;

    // Base class for all btree nodes. The btree data is store in a page.
    //
    // The node is structured like this on the page:
    //
    // Header
    //    Offset Size Description
    //    ------ ---- --------------------------------------------------------------------
    //     0      2   Cout of items stored on this node.
    //
    // Derived class specific header
    //    Derived class can extend the header to store values. They need to override the
    //    header_size() method to reserve the space needed.
    //
    // Items array
    //    The items array is a compact array containing key and value pairs.
    class Node
    {
    public:
        typedef std::uint16_t ItemsCount;
        typedef std::uint16_t ItemIndex;

        // Constructor
        //
        // if `initialize` is true, we initialize a new empty node. If it is false we
        // construct a node based on the data already existing in the page.
        Node(const BTree& btree,
             std::shared_ptr<pager::Page> page,
             common::KeyValueSize value_size,
             bool initialize);

        inline virtual ~Node() {};

        // Insert a new items in the node
        void Insert(common::ConstByteSpan key, common::ConstByteSpan value);

        // Check if there is space available for at least one more item
        bool CheckForSpace() const;

        // Returns the number of itemps in the node.
        inline ItemsCount items_count() const;

        // Returns the space available for new itemps on the page.
        inline common::FileSize available_space() const;

    protected:
        // Returns the size of the page header.
        virtual common::FileSize header_size() const;

        inline common::KeyValueSize key_size() const;
        inline common::KeyValueSize value_size() const;
        inline std::shared_ptr<pager::Page> page() const;

        inline common::ConstByteSpan GetItemSpan(ItemIndex index);
        inline common::ConstByteSpan GetItemKey(ItemIndex index);
        inline common::ConstByteSpan GetItemValue(ItemIndex index);

        // Search for the position of a key if the key is present in the node. It the key
        // is not presents, the position at which it would be inserted is returned. The
        // seconds element of the std::pair indicate if the key was found.
        std::pair<ItemIndex, bool> SearchIndex(common::ConstByteSpan key);

    private:
        static const common::FileSize ITEMS_COUNT_SIZE     = 2;

        static const common::FileOffset ITEMS_COUNT_OFFSET = 0;

        inline common::KeyValueSize item_size() const;
        inline common::ByteSpan items_count_span() const;
        inline common::ByteSpan items_array_span() const;

        inline void items_count(ItemsCount items_count);

        std::shared_ptr<pager::Page> page_;
        common::KeyValueSize key_size_;
        common::KeyValueSize value_size_;
    };

    Node::ItemsCount Node::items_count() const
    {
        return common::Deserialize<Node::ItemsCount>(items_count_span());
    }

    common::FileSize Node::available_space() const
    {
        return page_->content().size() - header_size() - items_count() * item_size();
    }

    common::KeyValueSize Node::key_size() const
    {
        return key_size_;
    }

    common::KeyValueSize Node::value_size() const
    {
        return value_size_;
    }

    std::shared_ptr<pager::Page> Node::page() const
    {
        return page_;
    }

    void Node::items_count(Node::ItemsCount items_count)
    {
        common::Serialize(items_count, items_count_span());
    }

    common::KeyValueSize Node::item_size() const
    {
        return key_size_ + value_size_;
    }

    common::ByteSpan Node::items_count_span() const
    {
        return page_->data().subspan(ITEMS_COUNT_OFFSET, ITEMS_COUNT_SIZE);
    }

    common::ByteSpan Node::items_array_span() const
    {
        return page_->content().subspan(header_size(), items_count() * item_size());
    }

    common::ConstByteSpan Node::GetItemSpan(ItemIndex index)
    {
        return items_array_span().subspan(index * item_size(), item_size());
    }

    common::ConstByteSpan Node::GetItemKey(ItemIndex index)
    {
        return GetItemSpan(index).subspan(0, key_size_);
    }

    common::ConstByteSpan Node::GetItemValue(ItemIndex index)
    {
        return GetItemSpan(index).subspan(key_size_, value_size_);
    }
} // namespace mkvdb::btree

#endif // MKVDB_BTREE_NODE_HPP_