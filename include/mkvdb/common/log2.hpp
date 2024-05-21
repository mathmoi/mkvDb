#ifndef MKVDB_COMMON_LOG2_HPP_
#define MKVDB_COMMON_LOG2_HPP_

#include <cassert>
#include <cstdint>

// Determine if we can use the MSVC++ Intrinsics.
#ifdef _MSC_VER
#    include <intrin.h>
#    pragma intrinsic(_BitScanReverse)
#    define MKVDB_USE_MSC_INTRINSICS
#endif

// Determine if we can use the gnu builtin
#if defined(__GNUC__)
#    if defined __has_builtin
#        if __has_builtin(__builtin_clzl)
#            define MKVDB_USE_GNUC_BUILTIN
#        endif
#    endif
#endif

namespace mkvdb::common
{
    // Get the integral log base 2 of a number (aka the position of the highest bit set).
    inline std::uint32_t log2(std::uint32_t mask)
    {
        // A : The value is not zero
        assert(mask != 0);

#if defined(MKVDB_USE_MSC_INTRINSICS)
        // This is the intrinsics version of GetMsb used with VC++
        unsigned long index;
        _BitScanReverse(&index, mask);
        return index;
#elif defined(MKVDB_USE_GNUC_BUILTIN)
        return 31 - __builtin_clz(mask);
#else
#    warning                                                                             \
      "This pure c++ version of BitScanReverse is provided as a last resort and might not be optimal. It might be helpfull to add a compiler specific version for your platform/compiler."
        static const int MultiplyDeBruijnBitPosition[32] = {
            0, 9,  1,  10, 13, 21, 2,  29, 11, 14, 16, 18, 22, 25, 3, 30,
            8, 12, 20, 28, 15, 17, 24, 7,  19, 27, 23, 6,  26, 5,  4, 31
        };

        mask |= mask >> 1;
        mask |= mask >> 2;
        mask |= mask >> 4;
        mask |= mask >> 8;
        mask |= mask >> 16;
        return MultiplyDeBruijnBitPosition[(uint32_t) (mask * 0x07C4ACDDU) >> 27];
#endif
    }
} // namespace mkvdb::common

#endif // MKVDB_COMMON_LOG2_HPP_