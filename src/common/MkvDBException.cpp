#include <string.h>

#include "mkvDb/common/MkvDBException.hpp"

namespace mkvdb::common
{
    MkvDBException::MkvDBException(std::string_view message)
    : std::runtime_error(message.data())
    {}

    void ThrowFromErrno(std::string_view message)
    {
        const size_t formated_message_size = 500;
        char formated_message[formated_message_size];

        int errno_saved = errno;
        char* str_error = strerror(errno_saved);

        std::snprintf(formated_message, formated_message_size, message.data(), errno_saved, str_error);
        
        throw MkvDBException(formated_message);
    }
}