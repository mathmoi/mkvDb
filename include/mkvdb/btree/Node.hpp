#ifndef MKVDB_BTREE_NODE_HPP_
#define MKVDB_BTREE_NODE_HPP_

#include "mkvdb/common/Types.hpp"

#include "mkvdb/pager/Page.hpp"

#include "NodeHeader.hpp"

namespace mkvdb::btree
{
    /// Base class for all nodes (internal and leaf) in a b+tree.
    class Node
    {
    public:
        /// Constructor.
        ///
        /// Parameters:
        ///  page: Page containing the nodes data.
        inline Node(pager::Page& page)
        : page_(page),
          header_(page_.content().subspan(0, NodeHeader::HEADER_SIZE))
        {
        }

        /// Initialize a new node
        void InitializeNewNode()
        {
            header_.size(0);
            header_.byte_size(0);
        }

        /// Returns the size of the node.
        /// @return The size of the node.
        NodeHeader::NodeSize size() const { return header_.size(); }

        /// Returns the byte size of the node.
        /// @return The byte size of the node.
        NodeHeader::ByteSize byte_size() const { return header_.byte_size(); }

        /// @brief Inserts a key/value pair into the node.
        /// @param key The key to insert.
        /// @param value The value to insert.
        void Insert(common::ConstByteSpan key, common::ConstByteSpan value);

    private:
        pager::Page& page_;
        NodeHeader header_;
    };
} // namespace mkvdb::btree

#endif // MKVDB_BTREE_NODE_HPP_