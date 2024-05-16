#ifndef MKVDB_TESTS_TEMPORARY_FILE_HPP_
#define MKVDB_TESTS_TEMPORARY_FILE_HPP_

#include <string>

namespace mkvdb::tests
{
    // This class provide a temporary filename (does not create the file). When
    // the object is destroyed if the file exists it deletes it.
    class TemporaryFile
    {
    public:
        // Constructor
        TemporaryFile();

        // Destructor
        ~TemporaryFile();

        // Returns the filename
        std::string filename() const;

    private:
        std::string filename_;

        static std::string GenerateRandomName();
    };
} // namespace mkvdb::tests

#endif // MKVDB_TESTS_TEMPORARY_FILE_HPP_