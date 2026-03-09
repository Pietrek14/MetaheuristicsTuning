#pragma once

#include <vector>

#include "series.h"
#include "sga_hyperparams.h"
#include "selection.h"
#include "crossover.h"
#include "mutation.h"


namespace sga
{
	template <typename T, size_t DIM>
	std::vector<hyperparams<T, DIM>> generate_hyperparams(
		const core::objective<T, DIM>& obj,
		size_t num_iterations,
		size_t population_size)
	{
		std::vector<sga::selection::selection_operator<T, DIM>> selections =
		{
			{ "tournament", sga::selection::tournament_selector<T, DIM>(3) },
			{ "roulette", sga::selection::roulette_wheel_selection<T, DIM> }
		};

		std::vector<sga::crossover::crossover_operator<T, DIM>> crossovers =
		{
			{ "single_point", sga::crossover::single_point_crossover<T, DIM> },
			{ "uniform", sga::crossover::uniform_crossover<T, DIM>  }
		};

		std::vector<sga::mutation::mutation_operator<T, DIM>> mutations =
		{
			{ "bit_flip", sga::mutation::bit_flip_mutation<T, DIM> },
			{ "inversion", sga::mutation::inversion_mutation<T, DIM> }
		};

		std::vector<float> crossover_chances
			= series::exponential(0.5f, 1.0f, 80);
		std::vector<float> mutation_chances
			= series::exponential(0.001f, 0.3f, 80);

		std::vector<hyperparams<T, DIM>> result;

		for (auto& selection : selections)
			for (auto& crossover : crossovers)
				for (auto& mutation : mutations)
					for (float crossover_chance : crossover_chances)
						for (float mutation_chance : mutation_chances)
							result.push_back({
								obj,
								num_iterations,
								population_size,
								selection,
								crossover,
								mutation,
								crossover_chance,
								mutation_chance
								});

		return result;
	}

}