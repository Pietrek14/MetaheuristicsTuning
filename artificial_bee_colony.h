#pragma once

#include <vector>
#include <limits>
#include <cmath>
#include <numeric>

#include "random.h"
#include "bee.h"
#include "abc_hyperparams.h"


namespace abc
{

	template <typename T, size_t DIM>
	struct artificial_bee_colony
	{
		hyperparams<T, DIM> state;

		T run()
		{
			T min_obj_value = std::numeric_limits<T>::max();

			std::vector<bee<T, DIM>> population(state.population_size);
			for (bee<T, DIM>& b : population)
			{
				random_vector(b.position,
					state.objective.bounds.first,
					state.objective.bounds.second);
				b.objective_value = state.objective.obj_fun(b.position);
				b.fitness = compute_fitness(b.objective_value);

				if (b.objective_value < min_obj_value)
					min_obj_value = b.objective_value;
			}

			for (size_t t{}; t < state.num_iterations; ++t)
			{
				for (size_t m = 0; m < population.size(); ++m)
				{
					auto& current = population[m];

					size_t k = random_index_other_than(
						population.size(), m);

					std::vector<size_t> dims = random_subset(DIM, state.mutation_dims);
					bee<T, DIM> candidate = current;

					for (size_t i{}; i < dims.size(); ++i)
					{
						candidate.position[dims[i]] = current.position[dims[i]] +
							random_value(T{ -state.phi0 }, T{ state.phi0 }) *
							(current.position[dims[i]] -
								population[k].position[dims[i]]);
						candidate.position[dims[i]] = std::clamp(
							candidate.position[dims[i]],
							state.objective.bounds.first,
							state.objective.bounds.second);
					}

					candidate.objective_value =
						state.objective.obj_fun(candidate.position);
					candidate.fitness =
						compute_fitness(candidate.objective_value);

					if (candidate.fitness > current.fitness)
					{
						current = candidate;
						current.trial = 0;
					}
					else
						current.trial++;

					if (current.objective_value < min_obj_value)
						min_obj_value = current.objective_value;
				}

				std::vector<size_t> indices =
					state.selection_policy.function(population);

				for (size_t n{}; n < population.size(); ++n)
				{
					auto& current = population[indices[n]];

					size_t k = random_index_other_than(
						population.size(), indices[n]);

					std::vector<size_t> dims = random_subset(DIM, state.mutation_dims);
					bee<T, DIM> candidate = current;

					for (size_t i{}; i < dims.size(); ++i)
					{
						candidate.position[dims[i]] = current.position[dims[i]] +
							random_value(T{ -state.phi0 }, T{ state.phi0 }) *
							(current.position[dims[i]] -
								population[k].position[dims[i]]);
						candidate.position[dims[i]] = std::clamp(
							candidate.position[dims[i]],
							state.objective.bounds.first,
							state.objective.bounds.second);
					}

					candidate.objective_value =
						state.objective.obj_fun(candidate.position);
					candidate.fitness =
						compute_fitness(candidate.objective_value);

					if (candidate.fitness > current.fitness)
					{
						current = candidate;
						current.trial = 0;
					}
					else
						current.trial++;

					if (current.objective_value < min_obj_value)
						min_obj_value = current.objective_value;
				}

				for (bee<T, DIM>& b : population)
				{
					if (b.trial >= state.limit)
					{
						random_vector(b.position,
							state.objective.bounds.first,
							state.objective.bounds.second);
						b.objective_value =
							state.objective.obj_fun(b.position);
						b.fitness =
							compute_fitness(b.objective_value);
						b.trial = 0;

						if (b.objective_value < min_obj_value)
							min_obj_value = b.objective_value;
					}
				}
			}

			return min_obj_value;
		}

	private:
		static T compute_fitness(T obj_value)
		{
			if (obj_value >= T{})
				return T{ 1 } / (T{ 1 } + obj_value);
			else
				return T{ 1 } + std::abs(obj_value);
		}
	};

}