#pragma once

#include <vector>

#include "series.h"
#include "cs_hyperparams.h"


namespace cs
{
	template <typename T, size_t DIM>
	std::vector<hyperparams<T, DIM>> generate_hyperparams(
		const core::objective<T, DIM>& obj,
		size_t num_iterations,
		size_t population_size)
	{
		std::vector<T> pa0s
			= series::linear(T{ 0.05 }, T{ 0.25 }, 5);
		std::vector<T> alpha0s
			= series::exponential(T{ 0.1 }, T{ 3.0 }, 80);
		std::vector<T> gammas
			= series::exponential(T{ 0.1 }, T{ 3.0 }, 80);

		std::vector<hyperparams<T, DIM>> result;

		for (T pa0 : pa0s)
			for (T alpha0 : alpha0s)
				for (T gamma : gammas)
					result.push_back({
						obj,
						num_iterations,
						population_size,
						pa0,
						alpha0,
						gamma
						});

		return result;
	}

}