#ifndef MKVDB_COMMON_TYPES_HPP_
#define MKVDB_COMMON_TYPES_HPP_

#include <cstddef>
#include <cstdint>
#include <span>

namespace mkvdb::common
{
    // Span of bytes
    typedef std::span<std::byte> ByteSpan;

    // Span of const bytes
    typedef std::span<const std::byte> ConstByteSpan;

    // Offset in a file
    typedef std::uint64_t FileOffset;

    // Offset in a page
    typedef std::uint16_t PageOffset;

    // Size of a key or values. Keys and Values can have a size of up to 2^32-1 bytes, so their size
    // is stored in a 32-bit unsigned integer.
    using ValueSize = std::uint32_t;

} // namespace mkvdb::common

#endif // MKVDB_COMMON_TYPES_HPP_