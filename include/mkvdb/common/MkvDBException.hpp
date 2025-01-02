#ifndef MKVDB_COMMON_MKVDB_EXCEPTION_HPP_
#define MKVDB_COMMON_MKVDB_EXCEPTION_HPP_

#include <stdexcept>

namespace mkvdb::common
{
    /// Exception raised by mkvDB in case of a runtime error
    class MkvDBException : public std::runtime_error
    {
    public:
        /// Constructor
        MkvDBException(std::string_view message);
    };

    /// Throw an exception based on the current errno.
    /// @param message The message parameter can contains up to two replacement fields representing
    /// the errno number (%1$d) and the errno description returned by "strerror(errno)" (%2$s).
    void ThrowFromErrno(std::string_view message = "%2$s ((%1$d))");
} // namespace mkvdb::common

#endif // MKVDB_COMMON_MKVDB_EXCEPTION_HPP_