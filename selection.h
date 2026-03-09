#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <iterator>
#include <string>
#include <functional>

#include "random.h"
#include "individual.h"
#include "print.h"
#include "trace_logger.h"


namespace sga::selection
{
	template <typename T, size_t DIM>
	using selection_function_t =
		std::function<std::vector<individual<T, DIM>>(
			const std::vector<individual<T, DIM>>& population,
			T worst_global,
			T best_global)>;


	template <typename T, size_t DIM>
	struct selection_operator
	{
		std::string name;
		selection_function_t<T, DIM> function;
	};


	template <typename T, size_t DIM>
	std::vector<individual<T, DIM>> roulette_wheel_selection(
		const std::vector<individual<T, DIM>>& population,
		T worst_global,
		T best_global)
	{
		std::vector<T> weights(population.size());
		std::vector<T> partial_sums(population.size());

		T denom = worst_global - best_global;
		for (size_t i{}; i < population.size(); ++i)
			weights[i] = denom > T{ 1e-6 } ?
				(worst_global - population[i].fitness()) / denom :
				T{ 1 };

		std::partial_sum(weights.begin(), weights.end(), partial_sums.begin());
		T weights_sum = partial_sums.back();

		std::vector<individual<T, DIM>> mating_pool;
		mating_pool.reserve(population.size());

		for (size_t j{}; j < population.size(); ++j)
		{
			T r = random_value(T{ 0 }, weights_sum);
			auto it = std::lower_bound(partial_sums.begin(), partial_sums.end(), r);
			size_t index = std::min<size_t>(
				std::distance(partial_sums.begin(), it), population.size() - 1);
			mating_pool.push_back(population[index]);
		}

		return mating_pool;
	}


	template <typename T, size_t DIM>
	struct tournament_selector
	{
		explicit tournament_selector(size_t tournament_size)
			: tournament_size(tournament_size)
		{}

		std::vector<individual<T, DIM>> operator()(
			const std::vector<individual<T, DIM>>& population,
			T worst_global,
			T best_global)
		{
			std::vector<individual<T, DIM>> mating_pool;
			mating_pool.reserve(population.size());

			for (size_t i{}; i < population.size(); ++i)
			{
				individual<T, DIM> best_individual =
					population[random_index<std::numeric_limits<size_t>::max()>() % population.size()];
				for (size_t j{}; j < tournament_size - 1; ++j)
				{
					individual<T, DIM> competitor =
						population[random_index<std::numeric_limits<size_t>::max()>() % population.size()];
					if (competitor.fitness() < best_individual.fitness())
						best_individual = competitor;
				}
				mating_pool.push_back(best_individual);
			}
			return mating_pool;
		}


	private:
		size_t tournament_size;
	};
}