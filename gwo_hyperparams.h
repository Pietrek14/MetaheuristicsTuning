#pragma once

#include <vector>
#include <string>

#include "objective.h"
#include "convergence_policy.h"
#include "weight_policy.h"


namespace gwo
{
	template <typename T, size_t DIM>
	struct hyperparams
	{
		core::objective<T, DIM> objective;

		size_t num_iterations;
		size_t population_size;

		size_t n_leaders;
		T a_factor;
		T c_factor;

		convergence_policy::convergence_policy<T> convergence_policy;
		weight_policy::weight_policy<T> weight_policy;

		inline static const std::vector<std::string> csv_headers =
		{
			"n_leaders",
			"a_factor",
			"c_factor",
			"convergence",
			"weight",
			"population_size",
			"num_iterations",
			"objective",
			"dimensionality"
		};

		std::vector<core::field_t> csv_values() const
		{
			return {
				{ "n_leaders", std::to_string(n_leaders) },
				{ "a_factor", std::to_string(a_factor) },
				{ "c_factor", std::to_string(c_factor) },
				{ "convergence", convergence_policy.name },
				{ "weight", weight_policy.name },
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
				{ "convergence", convergence_policy.name },
				{ "weight", weight_policy.name },
				{ "a_factor", std::to_string(a_factor) },
				{ "c_factor", std::to_string(c_factor) }
			};
		}
	};
}