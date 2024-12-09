#ifndef MKVDB_TESTS_UTILS_RANDOM_HPP_
#define MKVDB_TESTS_UTILS_RANDOM_HPP_

#include <initializer_list>
#include <iterator>
#include <random>

namespace mkvdb::tests::utils
{
    // Returns a random number generator that can be reused.
    inline std::mt19937& random_generator()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }

    // Choose a value at random from a initializer_list.
    template<typename T>
    T ChooseRandom(std::initializer_list<T> values)
    {
        std::uniform_int_distribution<> dist(0, values.size() - 1);
        auto it = values.begin();
        std::advance(it, dist(random_generator()));
        return *it;
    }

    // Choose an integer at random from a range.
    template<typename T>
    typename std::enable_if<std::is_integral<T>::value, T>::type ChooseRandom(T begin,
                                                                              T end)
    {
        std::uniform_int_distribution<T> dist(begin, end);
        return dist(random_generator());
    }

} // namespace mkvdb::tests::utils

#endif // MKVDB_TESTS_UTILS_RANDOM_HPP_