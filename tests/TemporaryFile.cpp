#include "TemporaryFile.hpp"

#include <algorithm>
#include <filesystem>
#include <random>

namespace mkvdb::tests
{
    TemporaryFile::TemporaryFile()
    {
        filename_ = std::filesystem::temp_directory_path() / GenerateRandomName();
    }

    TemporaryFile::~TemporaryFile()
    {
        if(std::filesystem::exists(filename_))
        {
            std::filesystem::remove(filename_);
        }
    }

    std::string TemporaryFile::filename() const
    {
        return filename_;
    }

    std::string TemporaryFile::GenerateRandomName()
    {
        const std::string chars = "abcdefghijklmnopqrstuvwxyz"
                                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "0123456789";

        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> distribution(0, chars.size() - 1);

        std::string random_name = "mkvdb-tests-";
        std::generate_n(std::back_inserter(random_name),
                        32,
                        [&]() { return chars[distribution(generator)]; });

        return random_name;
    }
} // namespace mkvdb::tests