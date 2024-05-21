#ifndef MKVDB_PAGER_PAGE_HPP_
#define MKVDB_PAGER_PAGE_HPP_

#include "mkvdb/common/Types.hpp"

#include <cstdint>
#include <memory>

namespace mkvdb::pager
{
    // Represents a page in a binary file.
    class Page
    {
    public:
        typedef common::FileOffset PageSize;
        typedef std::uint32_t PageIndex;

        // Constructor.
        //
        // Parameters:
        //  index : Index of the page in it's parent file.
        //  page_size : Size of the page in bytes.
        Page(PageIndex index, PageSize size);

        // Returns the index of the page in it's parent file.
        inline PageIndex index() const { return index_; };

        // Returns the size of the page.
        inline PageSize size() const { return size_; }

        // Returns a ByteSpan that covers the whole page.
        inline common::ByteSpan data() const
        {
            return common::ByteSpan(data_.get(), size_);
        }

        // Returns a ByteSpan that covers the usable portion of the page. For all pages,
        // except the first one this covers the whole page. For the first page a portion
        // at the begining of the page is reserved for the database header and is thus not
        // included in the content.
        common::ByteSpan content() const;

        // Indicate if the page has been marked as modified.
        inline bool is_modified() const { return is_modified_; }

        // Mark the page as modified.
        inline void MarkAsModified() { is_modified_ = true; }

        // Mark the page as unmodified.
        inline void MarkAsUnmodified() { is_modified_ = false; }

    private:
        PageIndex index_;
        PageSize size_;
        bool is_modified_;
        std::unique_ptr<std::byte[]> data_;
    };
} // namespace mkvdb::pager

#endif // MKVDB_PAGER_PAGE_HPP_