#ifndef MKVDB_PAGER_BUFFER_VIEW_HPP_
#define MKVDB_PAGER_BUFFER_VIEW_HPP_

#include "mkvdb/common/Types.hpp"

#include <cassert>

namespace mkvdb::pager
{
    // Represents a view on a modifiable buffer of bytes. When the buffer is modified through
    // this view, it is marked as such. This class provides functionality to create sub-views
    // of itself. Modifying a sub-view also marks the parent view as modified.
    class BufferView
    {
    public:
        // Creates a BufferView for the given data buffer. This BufferView has no parent.
        // Parameters:
        //   buffer: The byte span representing the data buffer to view.
        BufferView(common::ByteSpan buffer)
        : BufferView(buffer, nullptr)
        {
        }

        // Returns true if the BufferView is marked as modified.
        bool is_modified() const { return is_modified_; }

        // Returns a read-only span of the buffer.
        common::ConstByteSpan buffer() const { return buffer_; }

        // This methods allows the caller to modify the internal buffer by passing a mutator
        // function that will receive a non-const byte-span as an argument. The methods marks the
        // buffer and it's parent as modified.
        template<typename F>
        void Mutate(F mutator)
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
        common::ByteSpan buffer_;
        BufferView* parent_;
        bool is_modified_;

        BufferView(common::ByteSpan buffer, BufferView* parent)
        : buffer_(buffer),
          parent_(parent),
          is_modified_(false)
        {
        }

        void MarkAsModified()
        {
            is_modified_ = true;
            if(parent_ != nullptr)
            {
                parent_->MarkAsModified();
            }
        }
    };
} // namespace mkvdb::pager

#endif // #ifndef MKVDB_PAGER_BUFFER_VIEW_HPP_
