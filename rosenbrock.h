#pragma once

#include "objective.h"
#include "math.h"


namespace core
{
	template <typename T, size_t DIM>
	T rosenbrock_function(const std::array<T, DIM>& x)
	{
		constexpr T ONE_HUNDRED = T{ 100 };

		T result{};
		for (size_t i{}; i < DIM - 1; ++i)
		{
			T xi = x[i] - SQRT2<T>;
			T xi1 = x[i + 1] - SQRT2<T>;
			T term1 = xi1 - pow2(xi);
			T term2 = ONE<T> -xi;
			result += ONE_HUNDRED * pow2(term1) + pow2(term2);
		}

		return result;
	}


	template <typename T, size_t DIM>
	constexpr objective<T, DIM> rosenbrock_objective()
	{
		return objective<T, DIM>{
			{ T{ -5 + SQRT2<T> }, T{ 10 + SQRT2<T> } },
				rosenbrock_function<T, DIM>,
				"rosenbrock"
		};
	}
}