#pragma once

#include <vector>

#include "series.h"
#include "abc_hyperparams.h"


namespace abc
{

	template <typename T, size_t DIM>
	std::vector<hyperparams<T, DIM>> generate_hyperparams(
		const core::objective<T, DIM>& obj,
		size_t num_iterations,
		size_t population_size)
	{
		std::vector<policy::selection_policy<T, DIM>> selection_policies
		{
			policy::roulette_wheel<T, DIM>,
			policy::tournament_3<T, DIM>,
			policy::tournament_5<T, DIM>
		};
		std::vector<size_t> limits =
			series::linear(size_t{ 12 }, size_t{ 170 }, 80);
		std::vector<T> phi0s =
			series::exponential(T{ 0.2 }, T{ 4.0 }, 80);

		std::vector<hyperparams<T, DIM>> result;

		for (auto selection_policy: selection_policies)
			for (size_t limit : limits)
				for (T phi0 : phi0s)
				{
					hyperparams<T, DIM> params;
					params.objective = obj;
					params.num_iterations = num_iterations;
					params.population_size = population_size;
					params.selection_policy = selection_policy;
					params.limit = limit;
					params.mutation_dims = 1;
					params.phi0 = phi0;

					result.push_back(params);
				}

		return result;
	}

}