#pragma once

#include <vector>

#include "series.h"
#include "gtoa_hyperparams.h"


namespace gtoa
{

	template <typename T, size_t DIM>
	std::vector<hyperparams<T, DIM>> generate_hyperparams(
		const core::objective<T, DIM>& obj,
		size_t num_iterations,
		size_t population_size)
	{
		std::vector<T> teaching_factors =
			series::linear(T{ 0.5 }, T{ 2.5 }, 15);

		std::vector<hyperparams<T, DIM>> result;

		for (T teaching_factor : teaching_factors)
		{
			hyperparams<T, DIM> params;
			params.objective = obj;
			params.num_iterations = num_iterations;
			params.population_size = population_size;
			params.n_leaders = 3;
			params.teaching_factor = teaching_factor;

			result.push_back(params);
		}

		return result;
	}

}