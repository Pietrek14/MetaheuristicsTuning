#pragma once

#include "print.h"
#include "timer.h"
#include "statistics.h"
#include "test_functions.h"
#include "rosenbrock.h"
#include "rastrigin.h"
#include "simple_genetic_algorithm.h"
#include "crossover.h"
#include "mutation.h"
#include "selection.h"

#include <chrono>
#include <array>
#include <vector>
#include <tuple>
#include <variant>


void sga_testing()
{
	using T = float;
	constexpr size_t DIM = 3;

	size_t num_iters = 150;
	size_t population_size = 50;
	float crossover_chance =  0.9f;
	float mutation_chance = 0.015f;

	auto selection = sga_selection::tournament_selector<T, DIM>{ 3 };
	auto crossover = sga_crossover::single_point_crossover<T, DIM>;
	auto mutation = sga_mutation::bit_flip_mutation<T, DIM>;

	auto objective = rastrigin_objective<T, DIM>();

	const size_t num_trials = 1000;

	std::vector<T> results;
	results.reserve(num_trials);

	const auto start = std::chrono::steady_clock::now();

	for (size_t i{}; i < num_trials; ++i)
	{
		T result = simple_genetic_algorithm<T, DIM>(
			num_iters,
			population_size,
			crossover_chance,
			mutation_chance,
			selection,
			crossover,
			mutation,
			objective);
		results.push_back(result);
		println("trial {:03}: best value = {:.6f}", i + 1, result);
	}

	std::sort(results.begin(), results.end());
	size_t top_n = static_cast<size_t>(num_trials * T{ 0.8 });
	T sum = std::accumulate(results.begin(), results.begin() + top_n, T{ 0 });
	T average_top_80 = sum / top_n;

	println("average of top 80%: {}", average_top_80);

	const auto end = std::chrono::steady_clock::now();

	double total_duration_ms = std::chrono::duration<double, std::milli>(end - start).count();
	println("total time: {}", total_duration_ms);
}