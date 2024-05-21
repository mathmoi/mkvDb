#include "mkvdb/pager/Pager.hpp"

#include "mkvdb/pager/Header.hpp"

#include <memory>

namespace mkvdb::pager
{
    Pager::Pager(fs::IFile& file)
    : file_(file)
    {
        page_size_ = Header::ReadPageSize(file);
        header_.emplace(GetPage(0));
    }

    std::shared_ptr<Page> Pager::GetPage(Page::PageIndex index)
    {
        // Try to get the page from the cache.
        auto it = pages_cache_.find(index);
        if(it != pages_cache_.end())
        {
            return it->second;
        }

        // Read the file, add it to the cache and return it.
        auto page = std::make_shared<Page>(index, page_size_);
        file_.Read(page->data(), index * page_size_);
        pages_cache_.insert({ index, page });
        return page;
    }

    std::shared_ptr<Page> Pager::GetNewPage()
    {
        auto index = header_->pages_count();
        header_->pages_count(index + 1);

        auto page = std::make_shared<Page>(index, page_size_);
        pages_cache_.insert({ index, page });
        return page;
    }

    void Pager::WriteModifiedPages()
    {
        for(const auto& pair : pages_cache_)
        {
            if(pair.second->is_modified())
            {
                file_.Write(pair.second->data(), pair.second->index() * page_size_);
            }
        }
        file_.Sync();
    }

} // namespace mkvdb::pager