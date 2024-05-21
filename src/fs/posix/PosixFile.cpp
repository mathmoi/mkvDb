#include "mkvdb/fs/posix/PosixFile.hpp"

#include "mkvdb/common/MkvDBException.hpp"
#include "mkvdb/common/Types.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstddef>

namespace mkvdb::fs::posix
{
    PosixFile::PosixFile(std::string_view filename)
    : filename_(filename),
      fd_(INVALID_FD)
    {
    }

    PosixFile::~PosixFile()
    {
        if(fd_ != INVALID_FD)
        {
            Close();
        }
    }

    void PosixFile::Create()
    {
        if(fd_ != INVALID_FD)
        {
            throw common::MkvDBException(
              "Cannot create file, the file is already opened.");
        }

        int flags   = O_RDWR | O_CREAT | O_EXCL;
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

        int fd = open(filename_.c_str(), flags, mode);
        if(fd == -1)
        {
            common::ThrowFromErrno(
              "An error occured while creating the file: %2$s (%1$d).");
        }
        fd_ = fd;
    }

    void PosixFile::Open()
    {
        if(fd_ != INVALID_FD)
        {
            throw common::MkvDBException("Cannot open file, the file is already opened.");
        }

        int flags = O_RDWR;
        int fd    = open(filename_.c_str(), flags);
        if(fd == -1)
        {
            common::ThrowFromErrno(
              "An error occured while opening the file: %2$s (%1$d).");
        }
        fd_ = fd;
    }

    void PosixFile::Close()
    {
        if(fd_ == INVALID_FD)
        {
            throw common::MkvDBException(
              "Cannot close the file. The file is not opened.");
        }

        int result = close(fd_);
        fd_        = INVALID_FD;
        if(result == -1)
        {
            common::ThrowFromErrno(
              "An error occured while closing the file: %2$s (%1$d).");
        }
    }

    void PosixFile::Delete()
    {
        if(fd_ != INVALID_FD)
        {
            throw common::MkvDBException("Cannot delete the file, the file is opened.");
        }

        int result = remove(filename_.c_str());
        if(result == -1)
        {
            common::ThrowFromErrno(
              "An error occured while deleting the file: %2$s (%1$d).");
        }
    }

    void PosixFile::Write(common::ConstByteSpan buffer, common::FileOffset offset)
    {
        if(fd_ == INVALID_FD)
        {
            throw common::MkvDBException(
              "Cannot create write to file, the file is not opened.");
        }

        auto result = lseek(fd_, offset, SEEK_SET);
        if(result == -1)
        {
            common::ThrowFromErrno("An error occured writing to the file : %2$s (%1$d).");
        }

        auto bytes_written = write(fd_, buffer.data(), buffer.size_bytes());
        if(bytes_written == -1)
        {
            common::ThrowFromErrno("An error occured writing to the file : %2$s (%1$d).");
        }
        else if(static_cast<size_t>(bytes_written) < buffer.size_bytes())
        {
            throw mkvdb::common::MkvDBException(
              "An error occured writing to the file : The amount of bytes written to the file is less than the amount expected.");
        }
    }

    void PosixFile::Read(common::ByteSpan buffer, common::FileOffset offset)
    {
        if(fd_ == INVALID_FD)
        {
            throw common::MkvDBException(
              "Cannot read from file, the file is not opened.");
        }

        auto result = lseek(fd_, offset, SEEK_SET);
        if(result == -1)
        {
            common::ThrowFromErrno(
              "An error occured reading from the file : %2$s (%1$d).");
        }

        auto bytes_read = read(fd_, buffer.data(), buffer.size_bytes());
        if(bytes_read == -1)
        {
            common::ThrowFromErrno(
              "An error occured reading from the file : %2$s (%1$d).");
        }
        else if(static_cast<size_t>(bytes_read) < buffer.size_bytes())
        {
            throw common::MkvDBException(
              "An error occured reading from the file : The amount of bytes read from the file is less than expected.");
        }
    }

    void PosixFile::Sync()
    {
        auto result = fsync(fd_);
        if(result == -1)
        {
            common::ThrowFromErrno(
              "An error occured while syncing changes to the persistence medium : %2$s (%1$d).");
        }
    }

    common::FileOffset PosixFile::size() const
    {
        if(fd_ == INVALID_FD)
        {
            throw common::MkvDBException("Cannot get file size, the file is not opened.");
        }

        auto result = lseek(fd_, 0, SEEK_END);
        if(result == -1)
        {
            common::ThrowFromErrno(
              "An error occured while getting the file size : %2$s (%1$d).");
        }

        return result;
    }

} // namespace mkvdb::fs::posix