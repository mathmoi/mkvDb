#ifndef MKVDB_PAGER_PAGE_HPP_
#define MKVDB_PAGER_PAGE_HPP_

#include "mkvdb/common/Types.hpp"

#include "mkvdb/pager/BufferView.hpp"

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

        // Constructs a new page with uninitialized data.
        //
        // Parameters:
        //  index : Index of the page in it's parent file.
        //  page_size : Size of the page in bytes.
        Page(PageIndex index, PageSize size);

        // Construct a new page with existing data.
        //
        // Parameters:
        //  index : Index of the page in it's parent file.
        //  page_size : Size of the page in bytes.
        Page(PageIndex index, std::unique_ptr<std::byte[]>&& data, PageSize size);

        // Returns the index of the page in it's parent file.
        inline PageIndex index() const { return index_; };

        // Returns the size of the page.
        inline PageSize size() const { return buffer_view_.const_span().size(); }

        // Returns a BufferView convering the page's content.
        inline BufferView buffer_view() { return buffer_view_; };

    private:
        PageIndex index_;
        std::unique_ptr<std::byte[]> data_;
        BufferView buffer_view_;
    };
} // namespace mkvdb::pager

#endif // MKVDB_PAGER_PAGE_HPP_