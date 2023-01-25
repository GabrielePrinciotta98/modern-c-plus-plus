#pragma once

#include <cmath>
#include <numeric>

template <typename T>
constexpr T gcd(T arg1, T arg2)
{
    return std::gcd(arg1, arg2);
}

template <typename T, typename... Args>
constexpr T gcd(T arg1, T arg2, Args... args)
{   
    auto result = gcd(arg1, arg2);
    return gcd(result, args...);
}

template <typename T>
constexpr T mcm(T arg1, T arg2)
{
    return std::lcm(arg1, arg2);
}

template <typename T, typename... Args>
constexpr T mcm(T arg1, T arg2, Args... args)
{
    auto result = mcm(arg1, arg2);
    return mcm(result, args...);
}

template <int B, int E, int M>
struct Power {
    static constexpr int value = (B * Power<B, E - 1, M>::value) % M;
};

template <int B, int M>
struct Power <B, 1, M>{
    static constexpr int value = B % M;
};

template <int B, int M>
struct Power <B, 0, M>{
    static constexpr int value = 1 % M;
};

