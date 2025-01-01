#include "mkvdb/btree/SlotArray.hpp"

namespace mkvdb::btree
{
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
}