#pragma once

#include "objective.h"
#include "math.h"


namespace core
{
	template <typename T, size_t DIM>
	T griewank_function(const std::array<T, DIM>& x)
	{
		constexpr T FOUR_THOUSAND_INVERSE = ONE<T> / T{ 4000 };

		T sum_sq{};
		T prod_cos = ONE<T>;
		for (size_t i{}; i < DIM; ++i)
		{
			T xi = x[i] - SQRT2<T>;
			sum_sq += pow2(xi);
			prod_cos *= std::cos(xi / std::sqrt(static_cast<T>(i + 1)));
		}

		return sum_sq * FOUR_THOUSAND_INVERSE - prod_cos + ONE<T>;
	}


	template <typename T, size_t DIM>
	constexpr objective<T, DIM> griewank_objective()
	{
		return objective<T, DIM>{
			{ T{ -600 + SQRT2<T> }, T{ 600 + SQRT2<T> } },
				griewank_function<T, DIM>,
				"griewank"
		};
	}
}