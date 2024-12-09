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
    typedef std::int64_t FileOffset;

    // Size of a file or part of a file
    typedef std::uint64_t FileSize;

    // Type to represents the size of a key or a value.
    typedef std::uint64_t KeyValueSize;

} // namespace mkvdb::common

#endif // MKVDB_COMMON_TYPES_HPP_