#pragma once

#include "objective.h"
#include "math.h"


namespace core
{
	template <typename T, size_t DIM>
	T ackley_function(const std::array<T, DIM>& x)
	{
		constexpr T A = T{ 20 };
		constexpr T B = T{ 0.2 };
		constexpr T C = TWO_PI<T>;

		T sum_sq{};
		T sum_cos{};

		for (size_t i{}; i < DIM; ++i)
		{
			T xi = x[i] - SQRT2<T>;
			sum_sq += pow2(xi);
			sum_cos += std::cos(C * xi);
		}

		T term1 = -A * std::exp(-B * std::sqrt(sum_sq / DIM));
		T term2 = -std::exp(sum_cos / DIM);

		return term1 + term2 + A + std::exp(1);
	}


	template <typename T, size_t DIM>
	constexpr objective<T, DIM> ackley_objective()
	{
		return objective<T, DIM>{
			{ T{ -32.768 + SQRT2<T> }, T{ 32.768 + SQRT2<T> } },
				ackley_function<T, DIM>,
				"ackley"
		};
	}
}