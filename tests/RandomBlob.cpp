#include "RandomBlob.hpp"

#include <algorithm>
#include <random>

namespace mkvdb::tests
{
    RandomBlob::RandomBlob(size_t size)
    : data_(size)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);

        // Fill vector with random data
        std::generate(data_.begin(),
                      data_.end(),
                      [&]() { return static_cast<std::byte>(dis(gen)); });
    }
} // namespace mkvdb::tests