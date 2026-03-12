#pragma	once

#include <array>
#include <functional>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>

#include "individual.h"
#include "objective.h"
#include "print.h"
#include "trace_logger.h"
#include "selection.h"
#include "crossover.h"
#include "mutation.h"
#include "sga_hyperparams.h"

namespace sga
{

	template <typename T, size_t DIM>
	struct simple_genetic_algorithm
	{
		template <typename T1, size_t DIM1>
		using selection_function_t
			= sga::selection::selection_function_t<T1, DIM1>;

		template <typename T1, size_t DIM1>
		using crossover_function_t
			= sga::crossover::crossover_function_t<T1, DIM1>;

		template <typename T1, size_t DIM1>
		using mutation_function_t
			= sga::mutation::mutation_function_t<T1, DIM1>;


		hyperparams<T, DIM> state;

		T run()
		{
			T best_across_trials = std::numeric_limits<T>::max();

			std::vector<individual<T, DIM>> population;
			population.reserve(state.population_size);
			for (size_t i{}; i < state.population_size; ++i)
				population.emplace_back(state.objective);


			auto best_it = std::min_element(
				population.begin(), population.end(),
				[](const auto& ind1, const auto& ind2)
				{
					return ind1.fitness() < ind2.fitness();
				});
			std::array<T, DIM> best_global_position = best_it->phenotype();
			T best_global_fitness = best_it->fitness();

			auto worst_it = std::max_element(
				population.begin(), population.end(),
				[](const auto& ind1, const auto& ind2)
				{
					return ind1.fitness() < ind2.fitness();
				});
			std::array<T, DIM> worst_global_position = worst_it->phenotype();
			T worst_global_fitness = worst_it->fitness();


			auto update_best_worst = [&](const individual<T, DIM>& ind)
				{
					if (ind.fitness() < best_global_fitness)
					{
						best_global_position = ind.phenotype();
						best_global_fitness = ind.fitness();
						if (best_global_fitness < best_across_trials)
							best_across_trials = best_global_fitness;
					}
					else if (ind.fitness() > worst_global_fitness)
					{
						worst_global_position = ind.phenotype();
						worst_global_fitness = ind.fitness();
					}
				};


			std::vector<T> weights(state.population_size);
			std::vector<T> partial_sums(state.population_size);

			for (size_t i{}; i < state.num_iterations; ++i)
			{
				population = state.selection.function(
					population,
					worst_global_fitness,
					best_global_fitness);


				individual<T, DIM> best_individual
					= *std::min_element(population.begin(), population.end(),
					[](const auto& ind1, const auto& ind2)
					{
						return ind1.fitness() < ind2.fitness();
					});


				for (size_t j{}; j < state.population_size - 1; j += 2)
				{
					chromosome_t<T, DIM>& child1_chromosome
						= population[j].chromosome();
					chromosome_t<T, DIM>& child2_chromosome
						= population[j + 1].chromosome();

					state.crossover.function(
						child1_chromosome, child2_chromosome,
						state.crossover_chance);
					state.mutation.function(
						child1_chromosome,
						state.mutation_chance);
					state.mutation.function(
						child2_chromosome,
						state.mutation_chance);

					population[j].decode();
					population[j + 1].decode();
				}


				auto worst_it = std::max_element(
					population.begin(), population.end(),
					[](const auto& ind1, const auto& ind2)
					{
						return ind1.fitness() < ind2.fitness();
					});
				*worst_it = best_individual;


				best_global_fitness = std::numeric_limits<T>::max();
				worst_global_fitness = std::numeric_limits<T>::min();

				for (const individual<T, DIM>& ind : population)
					update_best_worst(ind);
			}

			return best_across_trials;
		}
	};

}