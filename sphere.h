#pragma once

#include "objective.h"
#include "math.h"


namespace core
{
	template <typename T, size_t DIM>
	T sphere_function(const std::array<T, DIM>& x)
	{
		T result{};
		for (size_t i{}; i < DIM; ++i)
		{
			T xi = x[i] - SQRT2<T>;
			result += pow2(xi);
		}

		return result;
	}


	template <typename T, size_t DIM>
	constexpr objective<T, DIM> sphere_objective()
	{
		return objective<T, DIM>{
			{ T{ -10 + SQRT2<T> }, T{ 10 + SQRT2<T> } },
				sphere_function<T, DIM>,
				"sphere"
		};
	}
}