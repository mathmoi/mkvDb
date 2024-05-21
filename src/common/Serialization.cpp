#include "mkvdb/common/Serialization.hpp"

#include "mkvdb/common/MkvDBException.hpp"

#include <format>

namespace mkvdb::common
{
    std::byte SerializeHex(char source)
    {
        if('0' <= source && source <= '9')
        {
            return static_cast<std::byte>(source - '0');
        }

        if('a' <= source && source <= 'f')
        {
            return static_cast<std::byte>(source - 'a' + 10);
        }

        if('A' <= source && source <= 'F')
        {
            return static_cast<std::byte>(source - 'A' + 10);
        }

        throw MkvDBException(std::format("Invalid hex digit: {}.", source));
    }

    void SerializeHex(std::string_view source, ByteSpan buffer)
    {
        if(source.size() % 2 != 0)
        {
            throw MkvDBException(
              "Cannont serilize source. The source must have an even length");
        }

        if(buffer.size_bytes() < source.size() / 2)
        {
            throw MkvDBException("Cannont serilize, the buffer is too small");
        }

        for(size_t x = 0; x < source.size(); x += 2)
        {
            auto high     = SerializeHex(source[x]);
            auto low      = SerializeHex(source[x + 1]);
            auto byte     = high << 4 | low;
            buffer[x / 2] = byte;
        }
    }

    std::vector<std::byte> SerializeHex(std::string_view source)
    {
        std::vector<std::byte> bytes;
        bytes.resize(source.size() / 2);
        SerializeHex(source, bytes);
        return bytes;
    }

    std::string DeserializeHex(ConstByteSpan source)
    {
        static const char hex_chars[] = "0123456789abcdef";

        std::string result;
        result.reserve(source.size() * 2);

        for(auto b : source)
        {
            result.push_back(hex_chars[std::to_integer<int>(b) >> 4]);
            result.push_back(hex_chars[std::to_integer<int>(b) & 0x0f]);
        }

        return result;
    }
} // namespace mkvdb::common