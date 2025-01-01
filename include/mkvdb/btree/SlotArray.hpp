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
        inline void Insert(std::uint16_t pos, std::uint16_t offset);

        // Get the offset at the given position.
        // @param pos Position of the offset to get. Must be less than the size of the array.
        inline std::uint16_t At(std::uint16_t pos) const;

        // Erase the offset at the given position.
        // @param pos Position of the offset to erase. Must be less than the size of the array.
        inline void Erase(std::uint16_t pos);

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

    void SlotArray::Insert(std::uint16_t pos, std::uint16_t offset)
    {
        assert(pos <= header_.size());
        assert(header_.unallocated_space() >= PAGE_OFFSET_SIZE);

        std::copy_backward(buffer_.begin() + pos * PAGE_OFFSET_SIZE,
                           buffer_.begin() + header_.size() * PAGE_OFFSET_SIZE,
                           buffer_.begin() + (header_.size() + 1) * PAGE_OFFSET_SIZE);

        common::Serialize(offset, buffer_.subspan(pos * PAGE_OFFSET_SIZE, PAGE_OFFSET_SIZE));

        header_.size(header_.size() + 1);
        header_.unallocated_space(header_.unallocated_space() - PAGE_OFFSET_SIZE);
    }

    std::uint16_t SlotArray::At(std::uint16_t pos) const
    {
        assert(pos < header_.size());

        return common::Deserialize<std::uint16_t>(
          buffer_.subspan(pos * PAGE_OFFSET_SIZE, PAGE_OFFSET_SIZE));
    }

    void SlotArray::Erase(std::uint16_t pos)
    {
        assert(pos < header_.size());

        std::copy(buffer_.begin() + (pos + 1) * PAGE_OFFSET_SIZE,
                  buffer_.begin() + header_.size() * PAGE_OFFSET_SIZE,
                  buffer_.begin() + pos * PAGE_OFFSET_SIZE);

        header_.size(header_.size() - 1);
        header_.unallocated_space(header_.unallocated_space() + PAGE_OFFSET_SIZE);
    }

} // namespace mkvdb::btree

#endif // MKVDB_BTREE_SLOT_ARRAY_HPP_