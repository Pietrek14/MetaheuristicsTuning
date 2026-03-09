#pragma once

#include <vector>
#include <string>

#include "objective.h"
#include "field.h"


namespace pso
{
	template <typename T, size_t DIM>
	struct hyperparams
	{
		core::objective<T, DIM> objective;

		size_t num_iterations;
		size_t population_size;

		T inertia;
		T acc_coef_1;
		T acc_coef_2;

		inline static const std::vector<std::string> csv_headers =
		{
			"inertia",
			"acc_coef_1",
			"acc_coef_2",
			"population_size",
			"num_iterations",
			"objective",
			"dimensionality"
		};

		std::vector<core::field_t> csv_values() const
		{
			return {
				{ "inertia", std::to_string(inertia) },
				{ "acc_coef_1", std::to_string(acc_coef_1) },
				{ "acc_coef_2", std::to_string(acc_coef_2) },
				{ "population_size", std::to_string(population_size) },
				{ "num_iterations", std::to_string(num_iterations) },
				{ "objective", objective.name },
				{ "dimensionality", std::to_string(DIM) }
			};
		}

		std::vector<core::field_t> log_values() const
		{
			return {
				{ "objective", objective.name },
				{ "dimensionality", std::to_string(DIM) },
				{ "inertia", std::to_string(inertia) },
				{ "acc_coef_1", std::to_string(acc_coef_1) },
				{ "acc_coef_2", std::to_string(acc_coef_2) }
			};
		}
	};
}