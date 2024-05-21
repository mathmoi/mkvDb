#ifndef MKVDB_FS_POSIX_POSIX_FILE_HPP_
#define MKVDB_FS_POSIX_POSIX_FILE_HPP_

#include "mkvdb/fs/IFile.hpp"

#include <string>
#include <string_view>

namespace mkvdb::fs::posix
{
    // Represents a File on a Posix system.
    class PosixFile : public IFile
    {
    public:
        // Constructor.
        PosixFile(std::string_view filename);

        // Destructor
        ~PosixFile();

        // Creates a new file.
        void Create();

        // Opens on existing file.
        void Open();

        // Close the file.
        void Close();

        /// Delete the file on disk. The file must be close. The file may be recreated.
        void Delete();

        // Write a block of data in the file at a specified offset.
        void Write(common::ConstByteSpan buffer, common::FileOffset offset);

        // Read a block of data from the file. The size of the block of data red is
        // determined by the size of the buffer. The amout of data requested must be
        // available, otherwise an exception is thrown. It is an error to try to read past
        // the end of the file.
        void Read(common::ByteSpan buffer, common::FileOffset offset);

        // Flush all changes to the disk so it will not be lost in case of a crash or
        // power failure.
        void Sync();

        // Get the file size.
        common::FileOffset size() const;

    private:
        const int INVALID_FD = -1;

        std::string filename_;
        int fd_;
    };
} // namespace mkvdb::fs::posix

#endif // MKVDB_FS_POSIX_POSIX_FILE_HPP_