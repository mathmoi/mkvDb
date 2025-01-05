#ifndef MKVDB_BTREE_NODE_HEADER_HPP_
#define MKVDB_BTREE_NODE_HEADER_HPP_

#include "mkvdb/common/Serialization.hpp"

#include <cstdint>

namespace mkvdb::btree
{
    /// Class representing the header part of a btree nodes.
    class NodeHeader
    {
    public:
        using NodeSize = std::uint16_t;
        using ByteSize = std::uint32_t;

        /// Size of the buffer needed to store the NodeHeader.
        static const common::FileOffset HEADER_SIZE = 8;

        /// Constructor.
        /// @param buffer Buffer where the NodeHeader read and write it's data. The buffer must be
        /// of HEADER_SIZE size.
        inline NodeHeader(common::ByteSpan buffer);

        /// Returns the number of items in the node.
        inline NodeSize size() const;

        /// Set the number of items in the node.
        inline void size(NodeSize new_byte_size);

        /// Returns the total payload size in bytes.
        inline ByteSize byte_size() const;

        /// Set the total payload size in bytes
        inline void byte_size(ByteSize new_byte_size);

        /// Returns the size of the unallocated space in the node. The unallocated space is the
        /// space between the slot array and the first cell content location. This should not be
        /// confused with the free space in the node.
        inline NodeSize unallocated_space() const;

        /// Set the size of the unallocated space in the node.
        inline void unallocated_space(NodeSize new_unallocated_space);

    private:
        static const common::FileOffset SIZE_SIZE              = 2;
        static const common::FileOffset BYTE_SIZE_SIZE         = 4;
        static const common::FileOffset UNALLOCATED_SPACE_SIZE = 2;

        static const common::FileOffset SIZE_OFFSET      = 0;
        static const common::FileOffset BYTE_SIZE_OFFSET = SIZE_OFFSET + SIZE_SIZE;
        static const common::FileOffset UNALLOCATED_SPACE_OFFSET =
          BYTE_SIZE_OFFSET + BYTE_SIZE_SIZE;

        common::ByteSpan buffer_;
    };

    NodeHeader::NodeHeader(common::ByteSpan buffer)
    {
        assert(buffer.size() == HEADER_SIZE);
        buffer_ = buffer;
    }

    NodeHeader::NodeSize NodeHeader::size() const
    {
        return common::Deserialize<NodeSize>(buffer_.subspan(SIZE_OFFSET, SIZE_SIZE));
    }

    void NodeHeader::size(NodeSize new_byte_size)
    {
        common::Serialize(new_byte_size, buffer_.subspan(SIZE_OFFSET, SIZE_SIZE));
    }

    NodeHeader::ByteSize NodeHeader::byte_size() const
    {
        return common::Deserialize<ByteSize>(buffer_.subspan(BYTE_SIZE_OFFSET, BYTE_SIZE_SIZE));
    }

    void NodeHeader::byte_size(NodeHeader::ByteSize new_byte_size)
    {
        common::Serialize(new_byte_size, buffer_.subspan(BYTE_SIZE_OFFSET, BYTE_SIZE_SIZE));
    }

    NodeHeader::NodeSize NodeHeader::unallocated_space() const
    {
        return common::Deserialize<NodeSize>(
          buffer_.subspan(UNALLOCATED_SPACE_OFFSET, UNALLOCATED_SPACE_SIZE));
    }

    void NodeHeader::unallocated_space(NodeHeader::NodeSize new_unallocated_space)
    {
        common::Serialize(new_unallocated_space,
                          buffer_.subspan(UNALLOCATED_SPACE_OFFSET, UNALLOCATED_SPACE_SIZE));
    }

} // namespace mkvdb::btree

#endif // MKVDB_BTREE_NODE_HEADER_HPP_