#ifndef MKVDB_BTREE_BTREE_HPP_
#define MKVDB_BTREE_BTREE_HPP_

#include "mkvdb/common/Types.hpp"

namespace mkvdb::btree
{
    // Represents a BTree stored in pages prvided by a Pager.
    class BTree
    {
    public:
        // Constructor
        BTree(common::KeyValueSize key_size, common::KeyValueSize value_size);

        // Returns the size of keys in the btree.
        inline common::KeyValueSize key_size() const { return key_size_; }

        // Returns the size of values in the btree.
        inline common::KeyValueSize value_size() const { return value_size_; }

    private:
        common::KeyValueSize key_size_;
        common::KeyValueSize value_size_;
    };

} // namespace mkvdb::btree

#endif // MKVDB_BTREE_BTREE_HPP_