#pragma once

#include "objective.h"
#include "math.h"


namespace core
{
	template <typename T, size_t DIM>
	T rastrigin_function(const std::array<T, DIM>& x)
	{
		constexpr T TEN = T{ 10 };

		T result{ TEN * DIM };
		for (size_t i{}; i < DIM; ++i)
		{
			T xi = x[i] - SQRT2<T>;
			result += pow2(xi) - TEN * std::cos(TWO_PI<T> *xi);
		}

		return result;
	}


	template <typename T, size_t DIM>
	constexpr objective<T, DIM> rastrigin_objective()
	{
		return objective<T, DIM>{
			{ T{ -5.12 + SQRT2<T> }, T{ 5.12 + SQRT2<T> } },
				rastrigin_function<T, DIM>,
				"rastrigin"
		};
	}
}