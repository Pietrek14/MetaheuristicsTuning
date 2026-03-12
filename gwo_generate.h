#pragma once

#include <vector>

#include "series.h"
#include "gwo_hyperparams.h"
#include "convergence_policy.h"


namespace gwo
{

	template <typename T, size_t DIM>
	std::vector<hyperparams<T, DIM>> generate_hyperparams(
		const core::objective<T, DIM>& obj,
		size_t num_iterations,
		size_t population_size)
	{
		std::vector<convergence_policy::convergence_policy<T>> convergence_policies =
		{
			convergence_policy::linear<T>,
			convergence_policy::quadratic<T>,
			convergence_policy::exponential_3<T>
		};
		std::vector<weight_policy::weight_policy<T>> weight_policies =
		{
			weight_policy::constant<T>,
			weight_policy::linear<T>,
			weight_policy::quadratic<T>
		};
		std::vector<T> a_factors =
			series::exponential(T{ 0.5 }, T{ 10 }, 80);
		std::vector<T> c_factors =
			series::exponential(T{ 0.5 }, T{ 10 }, 80);

		std::vector<hyperparams<T, DIM>> result;

		for (auto convergence_policy : convergence_policies)
			for (auto weight_policy : weight_policies)
				for (T a_factor : a_factors)
					for (T c_factor : c_factors)
					{
						hyperparams<T, DIM> params;
						params.objective = obj;
						params.num_iterations = num_iterations;
						params.population_size = population_size;
						params.n_leaders = 3;
						params.a_factor = a_factor;
						params.c_factor = c_factor;
						params.convergence_policy = convergence_policy;
						params.weight_policy = weight_policy;

						result.push_back(params);
					}

		return result;
	}

}