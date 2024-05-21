#ifndef MKVDB_PAGER_HEADER_HPP_
#define MKVDB_PAGER_HEADER_HPP_

#include "mkvdb/common/Serialization.hpp"
#include "mkvdb/common/Types.hpp"

#include "mkvdb/fs/IFile.hpp"

#include "mkvdb/pager/Page.hpp"

namespace mkvdb::pager
{
    // Represents the database header.
    //
    // The headers is structured like this :
    //
    //    Offset Size Description
    //    ------ ---- -------------------------------------------------------------------
    //     0      16  Magic string identifying this file as a mkvDB file. It's content
    //                is "mkvDB file v1\0\0\0". The ten first characters will never
    //                change. The version number part might change in future versions of
    //                the library.
    //     16     1   Log base 2 of page size. This value must be between 9 and 16. The
    //                page size of the data base can be calculated by shifting 0x1 left
    //                by this value.
    //     17     4   Size of the database file in pages.
    class Header
    {
    public:
        static const common::FileOffset HEADER_SIZE;

        // Read the page size from a file.
        static common::FileOffset ReadPageSize(fs::IFile& file);

        // Initialize the header of a new database
        // Input:
        //  page_size : The size of the pages. Must be a power of two between 512 and
        //              65536.
        static void Initialize(fs::IFile& file, Page::PageSize page_size);

        // Constructor.
        //
        // Input
        //  page: Reference to the first page of the database.
        inline Header(std::shared_ptr<Page> page)
        : page_(page)
        {
        }

        // Returns the number of pages
        inline Page::PageIndex pages_count() const;

        // Set the number of pages
        inline void pages_count(Page::PageIndex count);

    private:
        static const std::string MAGIC_STRING;

        static const common::FileOffset MAGIC_STRING_SIZE = 16;
        static const common::FileOffset PAGE_SIZE_SIZE    = 1;
        static const common::FileOffset PAGES_COUNT_SIZE  = 4;

        static const common::FileOffset MAGIC_STRING_OFFSET = 0;
        static const common::FileOffset PAGE_SIZE_OFFSET =
          MAGIC_STRING_OFFSET + MAGIC_STRING_SIZE;
        static const common::FileOffset PAGES_COUNT_OFFSET =
          PAGE_SIZE_OFFSET + PAGE_SIZE_SIZE;

        inline common::ByteSpan pages_count_span() const;

        std::shared_ptr<Page> page_;
    };

    common::ByteSpan Header::pages_count_span() const
    {
        return page_->data().subspan(PAGES_COUNT_OFFSET, PAGES_COUNT_SIZE);
    }

    Page::PageIndex Header::pages_count() const
    {
        return common::Deserialize<Page::PageIndex>(pages_count_span());
    }

    void Header::pages_count(Page::PageIndex count)
    {
        common::Serialize(count, pages_count_span());
        page_->MarkAsModified();
    }

} // namespace mkvdb::pager

#endif // MKVDB_PAGER_HEADER_HPP_