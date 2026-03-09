#pragma once

#include <vector>
#include <string>

#include "objective.h"
#include "field.h"


namespace cs
{
	template <typename T, size_t DIM>
	struct hyperparams
	{
		core::objective<T, DIM> objective;

		size_t num_iterations;
		size_t population_size;

		T pa0;
		T alpha0;
		T gamma;

		inline static const std::vector<std::string> csv_headers =
		{
			"pa0",
			"alpha0",
			"gamma",
			"population_size",
			"num_iterations",
			"objective",
			"dimensionality"
		};

		std::vector<core::field_t> csv_values() const
		{
			return {
				{ "pa0", std::to_string(pa0) },
				{ "alpha0", std::to_string(alpha0) },
				{ "gamma", std::to_string(gamma) },
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
				{ "pa0", std::to_string(pa0) },
				{ "alpha0", std::to_string(alpha0) },
				{ "gamma", std::to_string(gamma) }
			};
		}
	};
}