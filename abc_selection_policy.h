#pragma once

#include <functional>
#include <vector>
#include <string>

#include "bee.h"


namespace abc::policy
{

	template <typename T, size_t DIM>
	using selection_function_t =
		std::function<std::vector<size_t>(const std::vector<bee<T, DIM>>&)>;

	template <typename T, size_t DIM>
	struct selection_policy
	{
		std::string name;
		selection_function_t<T, DIM> function;
	};


	template <typename T, size_t DIM>
	std::vector<size_t> roulette_wheel_selection(
		const std::vector<bee<T, DIM>>& population)
	{
		std::vector<T> fitness(population.size());
		for (size_t i = 0; i < population.size(); ++i)
			fitness[i] = population[i].fitness;

		std::vector<T> cumsum(population.size());
		std::partial_sum(fitness.begin(), fitness.end(), cumsum.begin());
		T fitness_sum = cumsum.back();

		std::vector<size_t> indices(population.size());
		for (size_t i{}; i < indices.size(); ++i)
		{
			T r = random_value(T{}, fitness_sum);
			
			auto it = std::lower_bound(cumsum.begin(), cumsum.end(), r);
			indices[i] = std::min<size_t>(
				std::distance(cumsum.begin(), it), population.size() - 1);
		}

		return indices;
	}


	template <typename T, size_t DIM>
	struct tournament_selector
	{
		explicit tournament_selector(size_t tournament_size)
			: tournament_size(tournament_size)
		{}

		std::vector<size_t> operator()(
			const std::vector<bee<T, DIM>>& population)
		{
			std::vector<size_t> indices(population.size());

			for (size_t i{}; i < indices.size(); ++i)
			{
				size_t best_index = random_index(population.size());
				for (size_t j{}; j < tournament_size - 1; ++j)
				{
					size_t competitor_index = random_index(population.size());
					if (population[competitor_index].fitness > population[best_index].fitness)
						best_index = competitor_index;
				}

				indices[i] = best_index;
			}

			return indices;
		}

	private:
		size_t tournament_size;
	};


	template <typename T, size_t DIM>
	inline selection_policy<T, DIM> roulette_wheel
	{
		"roulette wheel",
		roulette_wheel_selection<T, DIM>
	};

	template <typename T, size_t DIM>
	inline selection_policy<T, DIM> tournament_3
	{
		"tournament 3",
		tournament_selector<T, DIM>(size_t{ 3 })
	};

	template <typename T, size_t DIM>
	inline selection_policy<T, DIM> tournament_5
	{
		"tournament 5",
		tournament_selector<T, DIM>(size_t{ 5 })
	};
}