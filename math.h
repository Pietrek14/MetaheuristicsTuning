#pragma once

#include <numbers>


namespace core
{
	template <typename T>
	constexpr T SQRT2 = std::numbers::sqrt2_v<T>;

	template <typename T>
	constexpr T E = std::numbers::e_v<T>;

	template <typename T>
	constexpr T PI = std::numbers::pi_v<T>;

	template <typename T>
	constexpr T TWO_PI = T{ 2 } *std::numbers::pi_v<T>;

	template <typename T>
	constexpr T ONE = T{ 1 };


	template <typename T>
	constexpr inline T pow2(T a)
	{
		return a * a;
	}
}