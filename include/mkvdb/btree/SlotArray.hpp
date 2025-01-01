#ifndef MKVDB_BTREE_SLOT_ARRAY_HPP_
#define MKVDB_BTREE_SLOT_ARRAY_HPP_

#include "NodeHeader.hpp"

namespace mkvdb::btree
{
    class SlotArray
    {
    public:
        // Constructor.
        //
        // @param header The header of the node.
        // @param content_buffer The buffer containing the content of the node.
        inline SlotArray(NodeHeader& header, common::ByteSpan content_buffer);

        // Insert a new offset in the slot array.
        // @param pos Position where to insert the offset. The position must be less or equal than
        // the size of the array.
        // @param offset Offset to insert.
        void Insert(std::uint16_t pos, std::uint16_t offset);

        // Get the offset at the given position.
        // @param pos Position of the offset to get. Must be less than the size of the array.
        std::uint16_t At(std::uint16_t pos) const;

        // Erase the offset at the given position.
        // @param pos Position of the offset to erase. Must be less than the size of the array.
        void Erase(std::uint16_t pos);

    private:
        // Size of a page offset in the slot array in bytes.
        static const common::FileOffset PAGE_OFFSET_SIZE = 2;

        NodeHeader& header_;
        common::ByteSpan buffer_;
    };

    SlotArray::SlotArray(NodeHeader& header, common::ByteSpan content_buffer)
    : header_(header),
      buffer_(content_buffer.subspan(NodeHeader::HEADER_SIZE, header.size() * PAGE_OFFSET_SIZE))
    {
    }

} // namespace mkvdb::btree

#endif // MKVDB_BTREE_SLOT_ARRAY_HPP_