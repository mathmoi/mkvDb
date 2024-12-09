#ifndef MKVDB_FS_IFILE_HPP_
#define MKVDB_FS_IFILE_HPP_

#include "mkvdb/common/Types.hpp"

#include <fcntl.h>

namespace mkvdb::fs
{
    // Interface of objects that can read/write into a file.
    class IFile
    {
    public:
        // Creates a new file.
        virtual void Create() = 0;

        // Opens on existing file.
        virtual void Open() = 0;

        // Close the file.
        virtual void Close() = 0;

        // Delete the file on disk. The file must be close. The file may be
        // recreated.
        virtual void Delete() = 0;

        // Write a block of data in the file at a specified offset.
        virtual void Write(common::ConstByteSpan buffer, common::FileOffset offset) = 0;

        // Read a block of data from the file. The size of the block of data red is
        // determined by the size of the buffer. The amout of data requested must be
        // available, otherwise an exception is thrown. It is an error to try to read past
        // the end of the file.
        virtual void Read(common::ByteSpan buffer, common::FileOffset offset) = 0;

        // Flush all changes to the disk so it will not be lost in case of a crash or
        // power failure.
        virtual void Sync() = 0;

        // Get the file size.
        virtual common::FileSize size() const = 0;
    };
} // namespace mkvdb::fs

#endif // MKVDB_FS_IFILE_HPP_