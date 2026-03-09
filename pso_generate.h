#pragma once

#include <vector>

#include "series.h"
#include "pso_hyperparams.h"


namespace pso
{
	template <typename T, size_t DIM>
	std::vector<hyperparams<T, DIM>> generate_hyperparams(
		const core::objective<T, DIM>& obj,
		size_t num_iterations,
		size_t population_size)
	{
		std::vector<T> inertias
			= series::linear(T{ 0.4 }, T{ 0.8 }, 5);
		std::vector<T> acc_coefs_1
			= series::exponential(T{ 0.1 }, T{ 3.0 }, 80);
		std::vector<T> acc_coefs_2
			= series::exponential(T{ 0.1 }, T{ 3.0 }, 80);

		std::vector<hyperparams<T, DIM>> result;

		for (T inertia : inertias)
			for (T acc_coef_1 : acc_coefs_1)
				for (T acc_coef_2 : acc_coefs_2)
					result.push_back({
						obj,
						num_iterations,
						population_size,
						inertia,
						acc_coef_1,
						acc_coef_2
						});

		return result;
	}

}