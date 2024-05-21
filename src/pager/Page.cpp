#include "mkvdb/pager/Page.hpp"

#include "mkvdb/common/Types.hpp"

#include "mkvdb/pager/Header.hpp"

#include <memory>

namespace mkvdb::pager
{
    Page::Page(PageIndex index, PageSize size)
    : index_(index),
      size_(size),
      is_modified_(false),
      data_(std::make_unique<std::byte[]>(size_))
    {
    }

    common::ByteSpan Page::content() const
    {
        auto begin = data_.get();
        auto end   = begin + size_;

        if(index_ == 0)
        {
            begin += Header::HEADER_SIZE;
        }

        return common::ByteSpan(begin, end);
    }
} // namespace mkvdb::pager