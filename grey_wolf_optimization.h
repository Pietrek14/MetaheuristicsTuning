#pragma once

#include <vector>
#include <array>
#include <limits>
#include <algorithm>
#include <numeric>
#include <utility>

#include "random.h"
#include "wolf.h"
#include "gwo_hyperparams.h"


namespace gwo
{

	template <typename T, size_t DIM>
	struct grey_wolf_optimization
	{
		hyperparams<T, DIM> state;

		T run()
		{
			std::pair<T, T> fitness_bounds = std::pair<T, T>(
				std::numeric_limits<T>::max(),
				std::numeric_limits<T>::min());

			std::vector<wolf<T, DIM>> population(state.population_size);
			for (wolf<T, DIM>& w: population)
			{
				random_vector(w.position,
					state.objective.bounds.first,
					state.objective.bounds.second);
				w.fitness = state.objective.obj_fun(w.position);

				if (w.fitness < fitness_bounds.first)
					fitness_bounds.first = w.fitness;

				if (w.fitness > fitness_bounds.second)
					fitness_bounds.second = w.fitness;
			}

			std::vector<typename std::vector<wolf<T, DIM>>::iterator>
				leaders(state.n_leaders);

			std::vector<T> weights(state.n_leaders);

			std::vector<std::array<T, DIM>> r1s(leaders.size());
			std::vector<std::array<T, DIM>> r2s(leaders.size());
			std::vector<std::array<T, DIM>> As(leaders.size());
			std::vector<std::array<T, DIM>> Cs(leaders.size());
			std::vector<std::array<T, DIM>> Ds(leaders.size());
			std::vector<std::array<T, DIM>> Xs(leaders.size());

			for (size_t t{}; t < state.num_iterations; ++t)
			{
				std::partial_sort(population.begin(),
					population.begin() + leaders.size(), population.end(),
					[](const wolf<T, DIM>& w1, const wolf<T, DIM>& w2)
					{
						return w1.fitness < w2.fitness;
					});
				for (size_t i{}; i < state.n_leaders; ++i)
					leaders[i] = population.begin() + i;

				for (size_t i{}; i < leaders.size(); ++i)
					weights[i] = state.weight_policy.function(leaders[i]->fitness, fitness_bounds);
				
				T a = state.a_factor *
					state.convergence_policy.function(t, state.num_iterations);

				for (auto it = population.begin() + leaders.size();
					it != population.end(); ++it)
				{
					for (std::array<T, DIM>& r1 : r1s)
						random_vector(r1, T{ 0 }, T{ 1 });
					for (std::array<T, DIM>& r2 : r2s)
						random_vector(r2, T{ 0 }, T{ 1 });

					for (size_t d{}; d < DIM; ++d)
					{
						for (size_t l{}; l < leaders.size(); ++l)
						{
							As[l][d] = T{ 2 } * a * r1s[l][d] - a;
							Cs[l][d] = T{ state.c_factor } * r2s[l][d];
							Ds[l][d] = std::abs(Cs[l][d] * leaders[l]->position[d]
								- it->position[d]);
							Xs[l][d] = leaders[l]->position[d] - As[l][d] * Ds[l][d];
						}

						T weighted_sum{};
						T weight_sum{};
						for (size_t l{}; l < leaders.size(); ++l)
						{
							weighted_sum += weights[l] * Xs[l][d];
							weight_sum += weights[l];
						}

						it->position[d] = weighted_sum / weight_sum;

						it->position[d] = std::clamp(it->position[d],
							state.objective.bounds.first,
							state.objective.bounds.second);
					}

					it->fitness = state.objective.obj_fun(it->position);
				}

				fitness_bounds.first = std::min_element(population.begin(), population.end(),
					[](const wolf<T, DIM>& w1, const wolf<T, DIM>& w2)
					{
						return w1.fitness < w2.fitness;
					})->fitness;

				fitness_bounds.second = std::max_element(population.begin(), population.end(),
					[](const wolf<T, DIM>& w1, const wolf<T, DIM>& w2)
					{
						return w1.fitness < w2.fitness;
					})->fitness;
			}

			return fitness_bounds.first;
		}
	};

}