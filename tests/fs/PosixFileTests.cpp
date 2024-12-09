#include "mkvdb/fs/posix/PosixFile.hpp"

#include "mkvdb/common/MkvDBException.hpp"
#include "mkvdb/common/Types.hpp"

#include "../utils/RandomBlob.hpp"
#include "../utils/TemporaryFile.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstddef>

namespace mkvdb::tests::fs
{
    using namespace mkvdb::fs::posix;

    TEST_CASE("PosixFileCreate_CreateAFile_NothingThrows")
    {
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());

        CHECK_NOTHROW(sut.Create());
        CHECK_NOTHROW(sut.Close());
    }

    TEST_CASE("PosixFileCreate_FileAlreadyOpened_Throws")
    {
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());
        sut.Create();

        CHECK_THROWS_AS(sut.Create(), mkvdb::common::MkvDBException);
    }

    TEST_CASE("PosixFileCreate_FileAlreadyExists_Throws")
    {
        utils::TemporaryFile temp_file;
        PosixFile file(temp_file.filename());
        file.Create();
        file.Close();

        PosixFile sut(temp_file.filename());

        CHECK_THROWS_AS(sut.Create(), mkvdb::common::MkvDBException);
    }

    TEST_CASE("PosixFileClose_FileIsNotOpened_Throws")
    {
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());

        CHECK_THROWS_AS(sut.Close(), mkvdb::common::MkvDBException);
    }

    TEST_CASE("PosixFileOpen_CreateAFileThenOpenIt_NothingThrows")
    {
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());

        CHECK_NOTHROW(sut.Create());
        CHECK_NOTHROW(sut.Close());
        CHECK_NOTHROW(sut.Open());
        CHECK_NOTHROW(sut.Close());
    }

    TEST_CASE("PosixFileDelete_FileIsOpened_throws")
    {
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());
        sut.Create();

        CHECK_THROWS(sut.Delete());
    }

    TEST_CASE("PosixFileDelete_FileDoesNotExists_throws")
    {
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());

        CHECK_THROWS(sut.Delete());
    }

    TEST_CASE("PosixFileWrite_NormalCase_NothingTrows")
    {
        utils::RandomBlob test_data;
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());
        sut.Create();

        CHECK_NOTHROW(sut.Write(test_data.data(), 0));
    }

    TEST_CASE("PosixFileWrite_FileNotOpened_Throws")
    {
        utils::RandomBlob test_data;
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());

        CHECK_THROWS_AS(sut.Write(test_data.data(), 0), mkvdb::common::MkvDBException);
    }

    TEST_CASE("PosixFileRead_FileNotOpened_Throws")
    {
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());
        std::vector<std::byte> buffer(1024);

        CHECK_THROWS_AS(
          sut.Read(mkvdb::common::ByteSpan(buffer.data(), buffer.size()), 0),
          mkvdb::common::MkvDBException);
    }

    TEST_CASE("PosixFileRead_NormalCase_DataIsRead")
    {
        utils::RandomBlob test_data;
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());
        sut.Create();
        sut.Write(test_data.data(), 0);
        std::vector<std::byte> result(test_data.size());

        sut.Read(mkvdb::common::ByteSpan(result.data(), result.size()), 0);

        REQUIRE(std::equal(test_data.begin(), test_data.end(), result.begin()));
    }

    TEST_CASE("PosixFileRead_ReadDataWithAnOffset_DataIsRead")
    {
        utils::RandomBlob test_data;
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());
        sut.Create();
        sut.Write(test_data.data(), 1024);
        std::vector<std::byte> result(test_data.size());

        sut.Read(mkvdb::common::ByteSpan(result.data(), result.size()), 1024);

        REQUIRE(std::equal(test_data.begin(), test_data.end(), result.begin()));
    }

    TEST_CASE("PosixFileSync_NormalCase_NothingThrows")
    {
        utils::RandomBlob test_data;
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());
        sut.Create();
        sut.Write(test_data.data(), 1024);

        CHECK_NOTHROW(sut.Sync());
    }

    TEST_CASE("PosixFileSize_NormalCase_ReturnsCorrectSize")
    {
        const mkvdb::common::FileSize expected = 256;
        utils::RandomBlob test_data(expected);
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());
        sut.Create();
        sut.Write(test_data.data(), 0);

        auto result = sut.size();

        REQUIRE(expected == result);
    }

    TEST_CASE("PosixFileSize_FileNotOpened_Throws")
    {
        utils::RandomBlob test_data;
        utils::TemporaryFile temp_file;
        PosixFile sut(temp_file.filename());
        sut.Create();
        sut.Close();

        CHECK_THROWS_AS(sut.size(), mkvdb::common::MkvDBException);
    }
} // namespace mkvdb::tests::fs