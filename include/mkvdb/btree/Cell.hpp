#ifndef MKVDB_BTREE_CELL_HPP_
#define MKVDB_BTREE_CELL_HPP_

#include "mkvdb/common/Serialization.hpp"
#include "mkvdb/common/Types.hpp"

#include <cassert>

namespace mkvdb::btree
{
    /// Represents a cell in a B-Tree node. A cell is a structure that contains a key and a value of
    /// arbitrary sizes.
    class Cell
    {
    public:
        /// Constructor, creates a cell from a buffer.
        /// @pre The buffer must have exactly the size required to store the data it contains.
        inline Cell(common::ByteSpan buffer);

        /// Constructor, creates a new cell in the buffer with the given key and value.
        /// @param buffer Buffer where the cell will be stored.
        /// @param key Key of the cell.
        /// @param value Value of the cell.
        /// @pre The buffer must have exactly enough space to store the cell.
        Cell(common::ByteSpan buffer, common::ConstByteSpan key, common::ConstByteSpan value);

        /// Calculate the size required to store a cell with the given key and value sizes.
        static inline common::ValueSize CalculateRequiredSize(common::ValueSize key_size,
                                                              common::ValueSize value_size);

        /// Get the key of the cell.
        inline common::ConstByteSpan key() const;

        /// Get the value of the cell.
        inline common::ConstByteSpan value() const;

    private:
        static const size_t KEY_SIZE_SIZE   = sizeof(common::ValueSize);
        static const size_t VALUE_SIZE_SIZE = sizeof(common::ValueSize);

        static const size_t KEY_SIZE_OFFSET   = 0;
        static const size_t VALUE_SIZE_OFFSET = KEY_SIZE_OFFSET + KEY_SIZE_SIZE;
        static const size_t KEY_OFFSET        = VALUE_SIZE_OFFSET + VALUE_SIZE_SIZE;

        inline common::ValueSize key_size() const;
        inline void key_size(common::ValueSize key_size);
        inline common::ValueSize value_size() const;
        inline void value_size(common::ValueSize value_size);

        common::ByteSpan buffer_;
    };

    Cell::Cell(common::ByteSpan buffer)
    : buffer_(buffer)
    {
        assert(buffer_.size() >= KEY_SIZE_SIZE + VALUE_SIZE_SIZE);
        assert(buffer_.size() == KEY_SIZE_SIZE + VALUE_SIZE_SIZE + key_size() + value_size());
    }

    common::ValueSize Cell::key_size() const
    {
        return common::Deserialize<common::ValueSize>(
          buffer_.subspan(KEY_SIZE_OFFSET, KEY_SIZE_SIZE));
    }

    void Cell::key_size(common::ValueSize key_size)
    {
        common::Serialize<common::ValueSize>(key_size,
                                             buffer_.subspan(KEY_SIZE_OFFSET, KEY_SIZE_SIZE));
    }

    common::ValueSize Cell::value_size() const
    {
        return common::Deserialize<common::ValueSize>(
          buffer_.subspan(VALUE_SIZE_OFFSET, VALUE_SIZE_SIZE));
    }

    void Cell::value_size(common::ValueSize value_size)
    {
        common::Serialize<common::ValueSize>(value_size,
                                             buffer_.subspan(VALUE_SIZE_OFFSET, VALUE_SIZE_SIZE));
    }

    common::ValueSize Cell::CalculateRequiredSize(common::ValueSize key_size,
                                                  common::ValueSize value_size)
    {
        return KEY_SIZE_SIZE + VALUE_SIZE_SIZE + key_size + value_size;
    }

    common::ConstByteSpan Cell::key() const
    {
        return buffer_.subspan(KEY_OFFSET, key_size());
    }

    common::ConstByteSpan Cell::value() const
    {
        return buffer_.subspan(KEY_OFFSET + key_size(), value_size());
    }

} // namespace mkvdb::btree

#endif // MKVDB_BTREE_CELL_HPP_