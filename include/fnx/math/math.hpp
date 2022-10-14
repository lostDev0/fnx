#pragma once

#include<chrono>
#include<type_traits>

namespace fnx
{
    static constexpr auto PI = 3.14159265f;
    static constexpr auto PI_360 = PI * 2.f;
    static constexpr auto PI_90 = PI / 2.f;
    static constexpr auto RADIANS_TO_DEGREES = 180.f / PI;
    static constexpr auto DEGREES_TO_RADIANS = PI / 180.f;
    static constexpr auto EPSILON = 0.0001;

    template<typename T>
    /// @brief Produce a random value between an upper and lower bound.
    inline T random(T min, T max)
    {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::uniform_real_distribution<double> dist(min, max);
        std::mt19937_64 rng(seed);
        return static_cast<T>(dist(rng));
    }

    template<typename T, typename U = T>
    /// @brief Calculates the value from a ratio.
    inline T calc_value(T min, T max, U ratio)
    {
        // ratio of 0 returns min
        // ratio of 1 returns max
        return ((max - min) * ratio) + min;
    }

    template<typename T>
    /// @brief Calculates a ratio between values.
    inline T calc_ratio(T min, T max, T val)
    {
        return (val - min) / (max - min);
    }

    template<typename T, typename U>
    /// @brief Returns which ever value is lower.
    inline T minimum(T a, U b)
    {
        return a < b ? a : b;
    }

    template<typename T, typename U>
    /// @brief Returns which ever value is higher.
    inline T maximum(T a, U b)
    {
        return a > b ? a : b;
    }

    template<typename T, typename U>
    /// @brief Returns a value within the bounds
    inline T clamp(T val, U min, U max)
    {
        auto v = val;
        v = fnx::minimum(v, max);
        v = fnx::maximum(v, min);
        return v;
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
    inline T mod(const T& a, const T& b)
    {
        return a - ((a / b) * b);
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
    /// @brief Sums all numbers from n to 1.
    inline auto factorial_sum(T n) -> T
    {
        return ((n * n) + n) / T{ 2 };
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
    /// @brief Multiplies all numbers from n to 1.
    /// @note Don't call on a negative number.
    inline auto factorial(T n) -> T
    {
        T v{ 1 };
        for (T i = n; i > 0; --i)
        {
            v *= i;
        }
        return v;
    }
}