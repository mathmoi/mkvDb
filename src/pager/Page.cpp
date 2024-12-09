#include "mkvdb/pager/Page.hpp"

#include "mkvdb/common/Types.hpp"

#include <memory>

namespace mkvdb::pager
{
    Page::Page(PageIndex index, PageSize size)
    : index_(index),
      data_(std::make_unique<std::byte[]>(size)),
      buffer_view_(common::ByteSpan(data_.get(), size))
    {
    }

    Page::Page(PageIndex index, std::unique_ptr<std::byte[]>&& data, PageSize size)
    : index_(index),
      data_(std::move(data)),
      buffer_view_(common::ByteSpan(data_.get(), size))
    {
    }
} // namespace mkvdb::pager