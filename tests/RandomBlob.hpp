#ifndef MKVDB_TESTS_RANDOM_BLOB_HPP_
#define MKVDB_TESTS_RANDOM_BLOB_HPP_

#include <cstddef>
#include <span>
#include <vector>

namespace mkvdb::tests
{
    /// This class generates random block of bytes that can be used during tests.
    class RandomBlob
    {
    public:
        /// Constructor
        RandomBlob(size_t size = 1024);

        /// Returns a span that can be used to read the blob.
        inline std::span<const std::byte> data() const { return data_; }

        /// Returns the size of the blob.
        inline size_t size() const { return data_.size(); }

        /// Returns a const_iterator to the begin of the blob.
        inline auto begin() const { return data_.begin(); }

        /// Returns a const_iterator past the end of the blob.
        inline auto end() const { return data_.end(); }

    private:
        std::vector<std::byte> data_;
    };
} // namespace mkvdb::tests

#endif // MKVDB_TESTS_RANDOM_BLOB_HPP_