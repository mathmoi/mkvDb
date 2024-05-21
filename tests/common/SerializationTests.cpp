#include "mkvdb/common/Serialization.hpp"

#include "mkvdb/common/MkvDBException.hpp"
#include "mkvdb/common/Types.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

using namespace mkvdb::common;

TEST_CASE("ReverseBytes(uint16_t) returns the expected result")
{
    std::uint16_t input    = 0x0102;
    std::uint16_t expected = 0x0201;

    auto result = ReverseBytes(input);

    REQUIRE(expected == result);
}

TEST_CASE("ReverseBytes(uint32_t) returns the expected result")
{
    std::uint32_t input    = 0x01020304;
    std::uint32_t expected = 0x04030201;

    auto result = ReverseBytes(input);

    REQUIRE(expected == result);
}

TEST_CASE("ReverseBytes(uint64_t) returns the expected result")
{
    std::uint64_t input    = 0x0102030405060708;
    std::uint64_t expected = 0x0807060504030201;

    auto result = ReverseBytes(input);

    REQUIRE(expected == result);
}

TEST_CASE("Serialize(string_view) correctly serialize the string")
{
    const std::string input = "Hello World!";
    auto expected           = SerializeHex("48656c6c6f20576f726c642100");

    std::vector<std::byte> result(input.size() + 1);
    Serialize(input, result);

    REQUIRE_THAT(result, Catch::Matchers::RangeEquals(expected));
}

TEST_CASE("Serialize(std::uint8_t) correctly serialize the value")
{
    auto [input, expected] =
      GENERATE(std::make_tuple(static_cast<std::uint8_t>(0x00), SerializeHex("00")),
               std::make_tuple(static_cast<std::uint8_t>(0xae), SerializeHex("ae")),
               std::make_tuple(static_cast<std::uint8_t>(0xff), SerializeHex("ff")));

    std::vector<std::byte> result(1);
    Serialize(input, result);

    REQUIRE_THAT(result, Catch::Matchers::RangeEquals(expected));
}

TEST_CASE("Serialize(std::uint16_t) correctly serialize the value")
{
    auto [input, expected] =
      GENERATE(std::make_tuple(static_cast<std::uint16_t>(0x0000), SerializeHex("0000")),
               std::make_tuple(static_cast<std::uint16_t>(0xae35), SerializeHex("ae35")),
               std::make_tuple(static_cast<std::uint16_t>(0xffff), SerializeHex("ffff")));

    std::vector<std::byte> result(2);
    Serialize(input, result);

    REQUIRE_THAT(result, Catch::Matchers::RangeEquals(expected));
}

TEST_CASE("Serialize(std::uint32_t) correctly serialize the value")
{
    auto [input, expected] = GENERATE(
      std::make_tuple(static_cast<std::uint32_t>(0x00000000), SerializeHex("00000000")),
      std::make_tuple(static_cast<std::uint32_t>(0xae35f54d), SerializeHex("ae35f54d")),
      std::make_tuple(static_cast<std::uint32_t>(0xffffffff), SerializeHex("ffffffff")));

    std::vector<std::byte> result(4);
    Serialize(input, result);

    REQUIRE_THAT(result, Catch::Matchers::RangeEquals(expected));
}

TEST_CASE("Serialize(std::uint64_t) correctly serialize the value")
{
    auto [input, expected] =
      GENERATE(std::make_tuple(static_cast<std::uint64_t>(0x0000000000000000),
                               SerializeHex("0000000000000000")),
               std::make_tuple(static_cast<std::uint64_t>(0x4c4430f583c612d4),
                               SerializeHex("4c4430f583c612d4")),
               std::make_tuple(static_cast<std::uint64_t>(0xffffffffffffffff),
                               SerializeHex("ffffffffffffffff")));

    std::vector<std::byte> result(8);
    Serialize(input, result);

    REQUIRE_THAT(result, Catch::Matchers::RangeEquals(expected));
}

TEST_CASE("SerializeHex(char) correctly serialize all possible values")
{
    auto [input, expected] = GENERATE(std::make_tuple('0', std::byte(0)),
                                      std::make_tuple('1', std::byte(1)),
                                      std::make_tuple('2', std::byte(2)),
                                      std::make_tuple('3', std::byte(3)),
                                      std::make_tuple('4', std::byte(4)),
                                      std::make_tuple('5', std::byte(5)),
                                      std::make_tuple('6', std::byte(6)),
                                      std::make_tuple('7', std::byte(7)),
                                      std::make_tuple('8', std::byte(8)),
                                      std::make_tuple('9', std::byte(9)),
                                      std::make_tuple('a', std::byte(10)),
                                      std::make_tuple('b', std::byte(11)),
                                      std::make_tuple('c', std::byte(12)),
                                      std::make_tuple('d', std::byte(13)),
                                      std::make_tuple('e', std::byte(14)),
                                      std::make_tuple('f', std::byte(15)),
                                      std::make_tuple('A', std::byte(10)),
                                      std::make_tuple('B', std::byte(11)),
                                      std::make_tuple('C', std::byte(12)),
                                      std::make_tuple('D', std::byte(13)),
                                      std::make_tuple('E', std::byte(14)),
                                      std::make_tuple('F', std::byte(15)));

    auto result = SerializeHex(input);

    REQUIRE(expected == result);
}

TEST_CASE("SerializeHex(char) when passed an incorect value throws an exception")
{
    auto input = 'g';

    REQUIRE_THROWS_AS(SerializeHex(input), MkvDBException);
}

TEST_CASE("SerializeHex(std::string_view) corretly serialize the string")
{
    std::string input               = "00eaff";
    std::vector<std::byte> expected = { std::byte(0x00),
                                        std::byte(0xea),
                                        std::byte(0xff) };

    auto result = SerializeHex(input);

    REQUIRE_THAT(result, Catch::Matchers::RangeEquals(expected));
}

TEST_CASE("Deserialize<std::uint64_t>(...) returns the correct value")
{
    auto input    = SerializeHex("c46b710832c238fb");
    auto expected = UINT64_C(0xc46b710832c238fb);

    auto result = Deserialize<std::uint64_t>(input);

    REQUIRE(expected == result);
}

TEST_CASE("Deserialize<std::uint32_t>(...) returns the correct value")
{
    auto input    = SerializeHex("18c19205");
    auto expected = UINT32_C(0x18c19205);

    auto result = Deserialize<std::uint32_t>(input);

    REQUIRE(expected == result);
}

TEST_CASE("Deserialize<std::uint16_t>(...) returns the correct value")
{
    auto input    = SerializeHex("e06c");
    auto expected = UINT16_C(0xe06c);

    auto result = Deserialize<std::uint16_t>(input);

    REQUIRE(expected == result);
}

TEST_CASE("Deserialize<std::uint8_t>(...) returns the correct value")
{
    auto input    = SerializeHex("8c");
    auto expected = UINT8_C(0x8c);

    auto result = Deserialize<std::uint8_t>(input);

    REQUIRE(expected == result);
}

TEST_CASE("DeserializeHex(ConstByteSpan) returns the expected values")
{
    auto input = { std::byte(0xb4), std::byte(0x8a), std::byte(0xaa),
                   std::byte(0xd1), std::byte(0x3f), std::byte(0xdb),
                   std::byte(0x1e), std::byte(0x8e), std::byte(0xa1) };
    std::string expected("b48aaad13fdb1e8ea1");

    auto result = DeserializeHex(input);

    REQUIRE(result == expected);
}