#ifndef MKVDB_PAGER_PAGER_HPP_
#define MKVDB_PAGER_PAGER_HPP_

#include "mkvdb/fs/IFile.hpp"

#include "mkvdb/pager/Header.hpp"
#include "mkvdb/pager/Page.hpp"

#include <memory>
#include <optional>
#include <unordered_map>

namespace mkvdb::pager
{
    /// Class responsible for separating the database into pages that can be read and
    /// written as single blocks.
    class Pager
    {
    public:
        /// Constructor
        Pager(fs::IFile& file);

        /// Get a pointer to a specific page.
        std::shared_ptr<Page> GetPage(Page::PageIndex index);

        /// Returns a new page. The new page is either added at the end of the files or comme from a
        /// previously used page that is now on the free list. The content of the page is
        /// unspecified.
        std::shared_ptr<Page> GetNewPage();

        /// Write on disk the pages that are modified.
        void WriteModifiedPages();

    private:
        fs::IFile& file_;
        std::optional<Header> header_;
        Page::PageSize page_size_;
        std::unordered_map<Page::PageIndex, std::shared_ptr<Page>> pages_cache_;
    };
} // namespace mkvdb::pager

#endif /// MKVDB_PAGER_PAGER_HPP_