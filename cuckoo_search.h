#pragma once

#include <vector>
#include <array>
#include <limits>
#include <algorithm>

#include "objective.h"
#include "random.h"
#include "cuckoo.h"
#include "cs_hyperparams.h"


namespace cs
{

	template <typename T, size_t DIM>
	struct cuckoo_search
	{
		hyperparams<T, DIM> state;

		T run()
		{
			std::array<T, DIM> best_position;
			T best_fitness = std::numeric_limits<T>::max();

			std::vector<cuckoo<T, DIM>> cuckoos(state.population_size);
			for (cuckoo<T, DIM>& c : cuckoos)
			{
				random_vector(c.position,
					state.objective.bounds.first,
					state.objective.bounds.second);
				c.fitness = state.objective.obj_fun(c.position);

				if (c.fitness < best_fitness)
				{
					best_position = c.position;
					best_fitness = c.fitness;
				}
			}

			std::array<T, DIM> new_position{};
			T new_fitness{};
			size_t rand_idx{};

			T alpha{};
			T pa{};
			size_t worst_size{};

			for (size_t i{}; i < state.num_iterations; ++i)
			{
				alpha = state.alpha0 * std::exp(T{ -4 }
					* static_cast<T>(i)
					/ static_cast<T>(state.num_iterations));
				pa = state.pa0 * (T{ 1 }
					- static_cast<T>(i)
					/ static_cast<T>(state.num_iterations));
				worst_size = std::max(
					size_t{ 1 }, static_cast<size_t>(cuckoos.size() * pa));

				for (cuckoo<T, DIM>& c : cuckoos)
				{
					new_position = c.position;
					for (size_t d{}; d < DIM; ++d)
					{
						new_position[d] += alpha * levy<T>()
							+ state.gamma
							* (best_position[d] - c.position[d]);
						new_position[d] = std::clamp(new_position[d],
							state.objective.bounds.first,
							state.objective.bounds.second);
					}

					new_fitness = state.objective.obj_fun(new_position);
					rand_idx = random_index(cuckoos.size());

					if (new_fitness < cuckoos[rand_idx].fitness)
					{
						cuckoos[rand_idx].position = new_position;
						cuckoos[rand_idx].fitness = new_fitness;

						if (new_fitness < best_fitness)
						{
							best_position = new_position;
							best_fitness = new_fitness;
						}
					}
				}

				auto abandon_it = cuckoos.end() - worst_size;
				std::nth_element(cuckoos.begin(), abandon_it, cuckoos.end(),
					[](const cuckoo<T, DIM>& c1, const cuckoo<T, DIM>& c2)
					{
						return c1.fitness < c2.fitness;
					});

				for (auto it = abandon_it; it != cuckoos.end(); ++it)
				{
					for (size_t d{}; d < DIM; ++d)
					{
						it->position[d] = best_position[d] + T{ 0.1 }
							* (state.objective.bounds.second
								- state.objective.bounds.first)
							* normal<T>();
						it->position[d] = std::clamp(it->position[d],
							state.objective.bounds.first,
							state.objective.bounds.second);
					}
					it->fitness = state.objective.obj_fun(it->position);

					if (it->fitness < best_fitness)
					{
						best_position = it->position;
						best_fitness = it->fitness;
					}
				}
			}

			return best_fitness;
		}
	};

}