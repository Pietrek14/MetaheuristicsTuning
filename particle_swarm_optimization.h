#pragma once

#include <vector>
#include <array>
#include <limits>

#include "objective.h"
#include "random.h"
#include "particle.h"
#include "pso_hyperparams.h"


namespace pso
{
	template <typename T, size_t DIM>
	struct particle_swarm_optimization
	{
		hyperparams<T, DIM> state;

		T run()
		{
			constexpr T vmax_factor = T{ 0.2 };
			T vmax = (state.objective.bounds.second
				- state.objective.bounds.first)
				* vmax_factor;

			std::array<T, DIM> best_global_position;
			T best_global_fitness = std::numeric_limits<T>::max();

			std::vector<particle<T, DIM>> swarm(state.population_size);
			for (particle<T, DIM>& p : swarm)
			{
				random_vector(p.position,
					state.objective.bounds.first,
					state.objective.bounds.second);
				random_vector(p.velocity, -vmax, vmax);

				p.best_personal_position = p.position;
				p.best_personal_fitness
					= state.objective.obj_fun(p.position);

				if (p.best_personal_fitness < best_global_fitness)
				{
					best_global_position = p.best_personal_position;
					best_global_fitness = p.best_personal_fitness;
				}
			}

			std::array<T, DIM> r1{};
			std::array<T, DIM> r2{};

			for (size_t i{}; i < state.num_iterations; ++i)
			{
				for (particle<T, DIM>& p : swarm)
				{
					random_vector(r1, T{ 0.0 }, T{ 1.0 });
					random_vector(r2, T{ 0.0 }, T{ 1.0 });

					for (size_t d{}; d < DIM; ++d)
					{
						p.velocity[d] = p.velocity[d] * state.inertia
							+ (p.best_personal_position[d] - p.position[d])
							* (r1[d] * state.acc_coef_1)
							+ (best_global_position[d] - p.position[d])
							* (r2[d] * state.acc_coef_2);
						p.velocity[d] = std::clamp(p.velocity[d],
							-vmax, vmax);

						p.position[d] = p.position[d] + p.velocity[d];
						p.position[d] = std::clamp(p.position[d],
							state.objective.bounds.first,
							state.objective.bounds.second);
					}

					T fitness = state.objective.obj_fun(p.position);
					if (fitness < p.best_personal_fitness)
					{
						p.best_personal_position = p.position;
						p.best_personal_fitness = fitness;

						if (fitness < best_global_fitness)
						{
							best_global_position = p.best_personal_position;
							best_global_fitness = fitness;
						}
					}
				}
			}

			return best_global_fitness;
		}
	};
}