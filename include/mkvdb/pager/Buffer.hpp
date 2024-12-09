#ifndef MKVDB_COMMON_PAGER_BUFFER_HPP_
#define MKVDB_COMMON_PAGER_BUFFER_HPP_

#include "mkvdb/common/Types.hpp"

#include <cstddef>
#include <memory>

namespace mkvdb::pager
{

    class Buffer
    {
    public:
        // Returns true if the Buffer is marked as modified.
        bool is_modified() const { return is_modified_; }

        // Returns a read-only span of the buffer.
        common::ConstByteSpan const_span() const { return buffer_; }

        // This methods allows the caller to modify the internal buffer by passing a mutator
        // function that will receive a non-const byte-span as an argument. The methods marks the
        // buffer and it's parent as modified.
        void Mutate(std::function<void(common::ByteSpan buffer)> mutator)
        {
            mutator(buffer_);
            MarkAsModified();
        }

        // Returns a subview of the current BufferView. The newly created view is linked to the
        // current view as its parent. If the newly created view is ever marked as modified,
        // the current view will also be marked as modified. The reverse is not true; when a parent
        // view is modified, its children are not affected.
        //
        // Warning: The child view must not outlive the parent view. Violating this would result in
        // runtime errors.
        BufferView GetSubView(size_t offset, size_t size)
        {
            assert(0 < size);
            assert(offset < buffer_.size());
            assert(offset + size <= buffer_.size());

            return BufferView(buffer_.subspan(offset, size), this);
        }

    private:
        std::unique_ptr<std::byte[]> data_;
        common::FileOffset size_;
        bool is_modified_;
    };
} // namespace mkvdb::pager

#endif // MKVDB_COMMON_PAGER_BUFFER_HPP_