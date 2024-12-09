#include "mkvdb/fs/memory/MemoryFile.hpp"

#include "mkvdb/common/MkvDBException.hpp"
#include "mkvdb/common/Types.hpp"

namespace mkvdb::fs::memory
{
    MemoryFile::MemoryFile()
    : is_opened_(false)
    {
    }

    MemoryFile::MemoryFile(common::ConstByteSpan data)
    : is_opened_(false),
      data_(data.begin(), data.end())
    {
    }

    void MemoryFile::Create()
    {
        Open();
    }

    void MemoryFile::Open()
    {
        if(is_opened_)
        {
            throw common::MkvDBException("Cannot open file, the file is already opened.");
        }

        is_opened_ = true;
    }

    void MemoryFile::Close()
    {
        if(!is_opened_)
        {
            throw common::MkvDBException(
              "Cannot close the file. The file is not opened.");
        }

        is_opened_ = false;
    }

    void MemoryFile::Delete()
    {
        if(is_opened_)
        {
            throw common::MkvDBException("Cannot delete the file, the file is opened.");
        }

        data_.clear();
    }

    void MemoryFile::Write(common::ConstByteSpan buffer, common::FileOffset offset)
    {
        if(!is_opened_)
        {
            throw common::MkvDBException(
              "Cannot write to the file, the file is not opened.");
        }

        auto required_size = offset + buffer.size_bytes();
        if(data_.size() < required_size)
        {
            data_.resize(required_size);
        }

        std::copy(buffer.begin(), buffer.end(), data_.begin() + offset);
    }

    void MemoryFile::Read(common::ByteSpan buffer, common::FileOffset offset)
    {
        if(!is_opened_)
        {
            throw common::MkvDBException("Cannot read the file, the file is not opened.");
        }

        auto size_required = offset + buffer.size_bytes();
        if(data_.size() < size_required)
        {
            throw common::MkvDBException(
              "An error occured while reading the file : trying to read past the end of the file.");
        }

        std::copy(data_.begin() + offset,
                  data_.begin() + offset + buffer.size_bytes(),
                  buffer.data());
    }

    void MemoryFile::Sync()
    {
        // Nothing to do.
    }

    common::FileSize MemoryFile::size() const
    {
        if(!is_opened_)
        {
            throw common::MkvDBException("Cannot get file size, the file is not opened.");
        }

        return data_.size();
    }

} // namespace mkvdb::fs::memory