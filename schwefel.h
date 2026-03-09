#pragma once

#include "objective.h"
#include "math.h"


namespace core
{
	template <typename T, size_t DIM>
	T schwefel_function(const std::array<T, DIM>& x)
	{
		constexpr T A = T{ 418.9829 };

		T sum{};
		for (size_t i{}; i < DIM; ++i)
		{
			T xi = x[i] - SQRT2<T>;
			sum += xi * std::sin(std::sqrt(std::abs(xi)));
		}

		return A * DIM - sum;
	}


	template <typename T, size_t DIM>
	constexpr objective<T, DIM> schwefel_objective()
	{
		return objective<T, DIM>{
			{ T{ -500 + SQRT2<T> }, T{ 500 + SQRT2<T> } },
				schwefel_function<T, DIM>,
				"schwefel"
		};
	}
}