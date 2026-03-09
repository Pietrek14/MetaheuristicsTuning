#pragma once

#include <vector>
#include <string>

#include "selection.h"
#include "crossover.h"
#include "mutation.h"
#include "objective.h"


namespace sga
{
	template <typename T, size_t DIM>
	struct hyperparams
	{
		core::objective<T, DIM> objective;

		size_t num_iterations;
		size_t population_size;

		sga::selection::selection_operator<T, DIM> selection;
		sga::crossover::crossover_operator<T, DIM> crossover;
		sga::mutation::mutation_operator<T, DIM> mutation;

		float crossover_chance;
		float mutation_chance;

		inline static const std::vector<std::string> csv_headers =
		{
			"selection",
			"crossover",
			"mutation",
			"crossover_chance",
			"mutation_chance",
			"population_size",
			"num_iterations",
			"objective",
			"dimensionality"
		};

		std::vector<core::field_t> csv_values() const
		{
			return {
				{ "selection", selection.name },
				{ "crossover", crossover.name },
				{ "mutation", mutation.name },
				{ "crossover_chance", std::to_string(crossover_chance) },
				{ "mutation_chance", std::to_string(mutation_chance) },
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
				{ "crossover_chance", std::to_string(crossover_chance) },
				{ "mutation_chance", std::to_string(mutation_chance) }
			};
		}
	};
}