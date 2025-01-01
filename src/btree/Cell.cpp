#include "mkvdb/btree/Cell.hpp"

namespace mkvdb::btree
{
    Cell::Cell(common::ByteSpan buffer, common::ConstByteSpan key, common::ConstByteSpan value)
    : buffer_(buffer)
    {
        assert(buffer_.size() == CalculateRequiredSize(key.size(), value.size()));

        key_size(key.size());
        value_size(value.size());
        std::copy(key.begin(), key.end(), buffer_.begin() + KEY_OFFSET);
        std::copy(value.begin(), value.end(), buffer_.begin() + KEY_OFFSET + key.size());
    }
} // namespace mkvdb::btree