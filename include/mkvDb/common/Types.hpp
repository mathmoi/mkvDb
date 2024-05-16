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

} // namespace mkvdb::common

#endif // MKVDB_COMMON_TYPES_HPP_