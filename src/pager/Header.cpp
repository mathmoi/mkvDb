#include "mkvdb/pager/Header.hpp"

#include "mkvdb/common/Serialization.hpp"
#include "mkvdb/common/Types.hpp"
#include "mkvdb/common/log2.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace mkvdb::pager
{
    const common::FileOffset Header::HEADER_SIZE =
      Header::PAGES_COUNT_OFFSET + Header::PAGES_COUNT_SIZE;

    const std::string Header::MAGIC_STRING = "mkvDB file v1";

    common::FileOffset Header::ReadPageSize(fs::IFile& file)
    {
        std::array<std::byte, PAGE_SIZE_SIZE> buffer;
        file.Read(buffer, PAGE_SIZE_OFFSET);
        common::FileOffset log_2_page_size = common::Deserialize<std::uint8_t>(buffer);
        return 1 << log_2_page_size;
    }

    void Header::Initialize(fs::IFile& file, Page::PageSize page_size)
    {
        assert(512 <= page_size && page_size <= 65536);
        assert((page_size & (page_size - 1)) == 0); // page_size must be a power of two.

        std::vector<std::byte> page_bytes(page_size);
        common::ByteSpan page_span(page_bytes);

        // Magic string
        auto magic_string_span =
          page_span.subspan(MAGIC_STRING_OFFSET, MAGIC_STRING_SIZE);
        std::fill(magic_string_span.begin(), magic_string_span.end(), std::byte(0));
        common::Serialize(MAGIC_STRING, magic_string_span);

        // Page size
        auto page_size_span         = page_span.subspan(PAGE_SIZE_OFFSET, PAGE_SIZE_SIZE);
        std::uint8_t log2_page_size = common::log2(page_size);
        common::Serialize(log2_page_size, page_size_span);

        // Pages count
        auto page_count_span = page_span.subspan(PAGES_COUNT_OFFSET, PAGES_COUNT_SIZE);
        common::Serialize(static_cast<std::uint32_t>(1), page_count_span);

        file.Write(page_span, 0);
    }

} // namespace mkvdb::pager