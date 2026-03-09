#pragma once

#include <vector>
#include <string>

#include "objective.h"
#include "abc_selection_policy.h"


namespace abc
{

	template <typename T, size_t DIM>
	struct hyperparams
	{
		core::objective<T, DIM> objective;

		size_t num_iterations;
		size_t population_size;

		policy::selection_policy<T, DIM> selection_policy;
		size_t limit;
		size_t mutation_dims;
		T phi0;

		inline static const std::vector<std::string> csv_headers =
		{
			"selection",
			"limit",
			"mutation_dims",
			"phi0",
			"population_size",
			"num_iterations",
			"objective",
			"dimensionality"
		};

		std::vector<core::field_t> csv_values() const
		{
			return {
				{ "selection", selection_policy.name },
				{ "limit", std::to_string(limit) },
				{ "mutation_dims", std::to_string(mutation_dims) },
				{ "phi0", std::to_string(phi0) },
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
				{ "selection", selection_policy.name },
				{ "limit", std::to_string(limit) },
				{ "phi0", std::to_string(phi0) }
			};
		}
	};

}