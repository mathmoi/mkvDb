#include "mkvdb/fs/memory/MemoryFile.hpp"

#include "mkvdb/common/MkvDBException.hpp"
#include "mkvdb/common/Types.hpp"

#include "../utils/RandomBlob.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstddef>

namespace mkvdb::tests::fs
{
    using namespace mkvdb::fs::memory;

    TEST_CASE("MemoryFileCreate_CreateAFile_NothingThrows")
    {
        MemoryFile sut;

        CHECK_NOTHROW(sut.Create());
        CHECK_NOTHROW(sut.Close());
    }

    TEST_CASE("MemoryFileCreate_FileAlreadyOpened_Throws")
    {
        MemoryFile sut;
        sut.Create();

        CHECK_THROWS_AS(sut.Create(), mkvdb::common::MkvDBException);
    }

    TEST_CASE("MemoryFileClose_FileIsNotOpened_Throws")
    {
        MemoryFile sut;

        CHECK_THROWS_AS(sut.Close(), mkvdb::common::MkvDBException);
    }

    TEST_CASE("MemoryFileOpen_CreateAFileThenOpenIt_NothingThrows")
    {
        MemoryFile sut;

        CHECK_NOTHROW(sut.Create());
        CHECK_NOTHROW(sut.Close());
        CHECK_NOTHROW(sut.Open());
        CHECK_NOTHROW(sut.Close());
    }

    TEST_CASE("MemoryFileDelete_FileIsOpened_throws")
    {
        MemoryFile sut;
        sut.Create();

        CHECK_THROWS(sut.Delete());
    }

    TEST_CASE("MemoryFileWrite_NormalCase_NothingTrows")
    {
        utils::RandomBlob test_data;
        MemoryFile sut;
        sut.Create();

        CHECK_NOTHROW(sut.Write(test_data.data(), 0));
    }

    TEST_CASE("MemoryFileWrite_FileNotOpened_Throws")
    {
        utils::RandomBlob test_data;
        MemoryFile sut;

        CHECK_THROWS_AS(sut.Write(test_data.data(), 0), mkvdb::common::MkvDBException);
    }

    TEST_CASE("MemoryFileRead_FileNotOpened_Throws")
    {
        MemoryFile sut;
        std::vector<std::byte> buffer(1024);

        CHECK_THROWS_AS(
          sut.Read(mkvdb::common::ByteSpan(buffer.data(), buffer.size()), 0),
          mkvdb::common::MkvDBException);
    }

    TEST_CASE("MemoryFileRead_NormalCase_DataIsRead")
    {
        utils::RandomBlob test_data;
        MemoryFile sut;
        sut.Create();
        sut.Write(test_data.data(), 0);
        std::vector<std::byte> result(test_data.size());

        sut.Read(mkvdb::common::ByteSpan(result.data(), result.size()), 0);

        REQUIRE(std::equal(test_data.begin(), test_data.end(), result.begin()));
    }

    TEST_CASE("MemoryFileRead_ReadDataWithAnOffset_DataIsRead")
    {
        utils::RandomBlob test_data;
        MemoryFile sut;
        sut.Create();
        sut.Write(test_data.data(), 1024);
        std::vector<std::byte> result(test_data.size());

        sut.Read(mkvdb::common::ByteSpan(result.data(), result.size()), 1024);

        REQUIRE(std::equal(test_data.begin(), test_data.end(), result.begin()));
    }

    TEST_CASE("MemoryFileSync_NormalCase_NothingThrows")
    {
        utils::RandomBlob test_data;
        MemoryFile sut;
        sut.Create();
        sut.Write(test_data.data(), 1024);

        CHECK_NOTHROW(sut.Sync());
    }

    TEST_CASE("MemoryFileSize_NormalCase_ReturnsCorrectSize")
    {
        const mkvdb::common::FileSize expected = 256;
        utils::RandomBlob test_data(expected);
        MemoryFile sut;
        sut.Create();
        sut.Write(test_data.data(), 0);

        auto result = sut.size();

        REQUIRE(expected == result);
    }

    TEST_CASE("MemoryFileSize_FileNotOpened_Throws")
    {
        utils::RandomBlob test_data;
        MemoryFile sut;
        sut.Create();
        sut.Close();

        CHECK_THROWS_AS(sut.size(), mkvdb::common::MkvDBException);
    }
} // namespace mkvdb::tests::fs