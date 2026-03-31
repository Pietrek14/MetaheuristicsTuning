#pragma once

#include <vector>
#include <string>

#include "objective.h"


namespace gtoa
{
	template <typename T, size_t DIM>
	struct hyperparams
	{
		core::objective<T, DIM> objective;

		size_t num_iterations;
		size_t population_size;

		// according to the authors, should be either 1 or 2
		T teaching_factor;
		size_t n_leaders;

		inline static const std::vector<std::string> csv_headers =
		{
			"teaching_factor",
			"n_leaders",
			"population_size",
			"num_iterations",
			"objective",
			"dimensionality"
		};

		std::vector<core::field_t> csv_values() const
		{
			return {
				{ "teaching_factor", std::to_string(teaching_factor) },
				{ "n_leaders", std::to_string(n_leaders) },
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
				{ "teaching_factor", std::to_string(teaching_factor) },
			};
		}
	};
}