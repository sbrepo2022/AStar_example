#include <concepts>
#include <span>

template<typename T>
concept numeric = std::integral<T> || std::floating_point<T>;


template <numeric T, unsigned long UL>
constexpr auto manhattan_dist(const std::span<T, UL> p1, const std::span<T, UL> p2) -> T
{
    T dist = static_cast<T>(0.0);
    for (size_t i = 0; i < p1.size(); i++)
    {
        dist += p1[i] >= p2[i] ? p1[i] - p2[i] : p2[i] - p1[i];
    }
    return dist;
}
