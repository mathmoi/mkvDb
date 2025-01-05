#ifndef MKVDB_COMMON_TYPES_HPP_
#define MKVDB_COMMON_TYPES_HPP_

#include <cstddef>
#include <cstdint>
#include <span>

namespace mkvdb::common
{
    /// Span of bytes
    using ByteSpan = std::span<std::byte>;

    /// Span of const bytes
    using ConstByteSpan = std::span<const std::byte>;

    /// Offset in a file
    using FileOffset = std::uint64_t;

    /// Offset in a page
    using PageOffset = std::uint16_t;

    /// Size of a key or values. Keys and Values can have a size of up to 2^32-1 bytes, so their
    /// size is stored in a 32-bit unsigned integer.
    using ValueSize = std::uint32_t;

} // namespace mkvdb::common

#endif // MKVDB_COMMON_TYPES_HPP_