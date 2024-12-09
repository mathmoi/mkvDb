#ifndef MKVDB_FS_MEMORY_MEMORY_FILE_HPP_
#define MKVDB_FS_MEMORY_MEMORY_FILE_HPP_

#include "mkvdb/common/Types.hpp"

#include "mkvdb/fs/IFile.hpp"

#include <vector>

namespace mkvdb::fs::memory
{
    // Represents an in memory file.
    class MemoryFile : public IFile
    {
    public:
        // Construct an in memory empty file.
        MemoryFile();

        // Construct an in memory file with initial data.
        MemoryFile(common::ConstByteSpan data);

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
        common::FileSize size() const;

        // Returns a bytespan on the content of the file.
        inline common::ConstByteSpan data() const { return data_; }

    private:
        bool is_opened_;
        std::vector<std::byte> data_;
    };
} // namespace mkvdb::fs::memory

#endif // MKVDB_FS_MEMORY_MEMORY_FILE_HPP_