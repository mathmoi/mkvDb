#include "mkvdb/btree/BTree.hpp"

namespace mkvdb::btree
{
    BTree::BTree(common::KeyValueSize key_size, common::KeyValueSize value_size)
    : key_size_(key_size),
      value_size_(value_size)
    {
    }

} // namespace mkvdb::btree
