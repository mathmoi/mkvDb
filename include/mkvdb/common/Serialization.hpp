#ifndef MKVDB_COMMON_SERIALIZATION_HPP_
#define MKVDB_COMMON_SERIALIZATION_HPP_

#include "mkvdb/common/Types.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string_view>
#include <vector>

// Determine if we can use the MSVC++ Intrinsics.
#ifdef _MSC_VER
#    include <intrin.h>
#    pragma intrinsic(_BitScanReverse)
#    define MKVDB_USE_MSC_INTRINSICS

#    include <cstdlib>
#    define MKVDB_USE_MSC_SWAP
#endif

// Determine if we can use the gnu builtin
#if defined(__GNUC__)
#    if defined __has_builtin
#        if __has_builtin(__builtin_clzl)
#            define MKVDB_USE_GNUC_BUILTIN
#        endif
#    endif
#endif

#if defined(__GNUC__) || defined(__CLANG__)
#    define MKVDB_USE_GNUC_SWAP
#endif

namespace mkvdb::common
{
    // Swap the bytes of a 16 bits integer.
    inline std::uint16_t ReverseBytes(std::uint16_t v)
    {
#if defined(MKVDB_USE_MSC_SWAP)
        return _byteswap_ushort(v);
#elif defined(MKVDB_USE_GNUC_SWAP)
        return __builtin_bswap16(v);
#else
#    warning                                                                                       \
      "This pure c++ version of ReverseBytes is provided as a last resort and might not be optimal. It might be helpfull to add a compiler specific version for your platform/compiler."
        return ((v & 0xff00) >> 8) | ((v & 0x00ff) << 8);
#endif
    }

    // Swap the bytes of a 32 bits integer.
    inline std::uint32_t ReverseBytes(std::uint32_t v)
    {
#if defined(MKVDB_USE_MSC_SWAP)
        return _byteswap_ulong(v);
#elif defined(MKVDB_USE_GNUC_SWAP)
        return __builtin_bswap32(v);
#else
#    warning                                                                                       \
      "This pure c++ version of ReverseBytes is provided as a last resort and might not be optimal. It might be helpfull to add a compiler specific version for your platform/compiler."
        v = ((v & 0xff00ff00) >> 8) | ((v & 0x00ff00ff) << 8);
        v = ((v & 0xffff0000) >> 16) | ((v & 0x0000ffff) << 16);
        return v;
#endif
    }

    // Swap the bytes of a 64 bits integer.
    inline std::uint64_t ReverseBytes(std::uint64_t v)
    {
#if defined(MKVDB_USE_MSC_SWAP)
        return _byteswap_uint64(v);
#elif defined(MKVDB_USE_GNUC_SWAP)
        return __builtin_bswap64(v);
#else
#    warning                                                                                       \
      "This pure c++ version of ReverseBytes is provided as a last resort and might not be optimal. It might be helpfull to add a compiler specific version for your platform/compiler."
        v = ((v & UINT64_C(0xff00ff00ff00ff00)) >> 8) | ((v & UINT64_C(0x00ff00ff00ff00ff)) << 8);
        v = ((v & UINT64_C(0xffff0000ffff0000)) >> 16) | ((v & UINT64_C(0x0000ffff0000ffff)) << 16);
        v = ((v & UINT64_C(0xffffffff00000000)) >> 32) | ((v & UINT64_C(0x00000000ffffffff)) << 32);
        return v;
#endif
    }

    // This method prevents implicit conversion while calling the type specific
    // ReverseBytes functions.
    template<class T>
    void ReverseBytes(T) = delete;

    // Serialize a string in an array of byte.
    inline void Serialize(std::string_view source, ByteSpan buffer)
    {
        assert(source.length() + 1 <= buffer.size_bytes());

        std::strncpy((char*) buffer.data(), source.data(), buffer.size_bytes());
    }

    // Serialize an unsigned integer. The bytes written are in big-endian order no matter
    // what the order of the system is.
    template<typename Integer,
             std::enable_if_t<std::is_integral<Integer>::value, bool>            = true,
             std::enable_if_t<!std::is_signed<Integer>::value, bool>             = true,
             std::enable_if_t<!std::is_same<Integer, std::uint8_t>::value, bool> = true>
    inline void Serialize(Integer source, ByteSpan buffer)
    {
        assert(sizeof(Integer) <= buffer.size_bytes());

        if(std::endian::native == std::endian::little)
        {
            source = ReverseBytes(source);
        }

        std::byte* ptr = reinterpret_cast<std::byte*>(&source);
        std::copy(ptr, ptr + sizeof(Integer), buffer.data());
    }

    // Serialize an 8 bits unsigned integer.
    template<typename Integer,
             std::enable_if_t<std::is_integral<Integer>::value, bool>           = true,
             std::enable_if_t<!std::is_signed<Integer>::value, bool>            = true,
             std::enable_if_t<std::is_same<Integer, std::uint8_t>::value, bool> = true>
    inline void Serialize(Integer source, ByteSpan buffer)
    {
        assert(sizeof(source) <= buffer.size_bytes());
        *buffer.data() = static_cast<std::byte>(source);
    }

    // Serialize a single character containing an hexa-decimal character into a byte.
    std::byte SerializeHex(char source);

    // Serialize a string containing an arbitrary long hexa-decimal value into a specified
    // buffer.
    void SerializeHex(std::string_view source, ByteSpan buffer);

    // Serialize a string containing an arbitrary long hexa-decimal value into a vector
    // of bytes.
    std::vector<std::byte> SerializeHex(std::string_view source);

    // Deserialize an unsigned integer. The bytes read are assumed to be in big-endian
    // order and their order is reversed if we are on a little-endian system.
    template<typename Integer,
             std::enable_if_t<std::is_integral<Integer>::value, bool>            = true,
             std::enable_if_t<!std::is_signed<Integer>::value, bool>             = true,
             std::enable_if_t<!std::is_same<Integer, std::uint8_t>::value, bool> = true>
    inline Integer Deserialize(ConstByteSpan buffer)
    {
        assert(sizeof(Integer) <= buffer.size_bytes());

        Integer value;
        std::copy(buffer.data(),
                  buffer.data() + sizeof(Integer),
                  reinterpret_cast<std::byte*>(&value));

        if(std::endian::native == std::endian::little)
        {
            value = ReverseBytes(value);
        }

        return value;
    }

    // Deserialize an 8 bits unsigned integer.
    template<typename Integer,
             std::enable_if_t<std::is_integral<Integer>::value, bool>           = true,
             std::enable_if_t<!std::is_signed<Integer>::value, bool>            = true,
             std::enable_if_t<std::is_same<Integer, std::uint8_t>::value, bool> = true>
    inline Integer Deserialize(ConstByteSpan buffer)
    {
        assert(sizeof(Integer) <= buffer.size_bytes());
        return static_cast<Integer>(buffer.data()[0]);
    }

    // Deserialize a ConstByteSpan into a string representing the source data in
    // hexadecimal.
    std::string DeserializeHex(ConstByteSpan source);

} // namespace mkvdb::common

#endif // MKVDB_COMMON_SERIALIZATION_HPP_