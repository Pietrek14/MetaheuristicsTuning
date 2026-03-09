#pragma once

#include "print.h"
#include "timer.h"
#include "statistics.h"
#include "particle_swarm_optimization.h"
#include "objective_factory.h"

#include <chrono>
#include <array>
#include <vector>
#include <tuple>
#include <variant>


void objective_testing()
{
	using T = double;
	constexpr size_t DIM = 6;


	T inertia{ 0.5 };
	T acc_coef_1{ 0.90 };
	T acc_coef_2{ 0.90 };
	size_t swarm_size = 120;
	size_t num_iters = 300;
	auto objective = core::make<T, DIM>("rastrigin");


	const size_t num_trials = 500;

	std::vector<T> results;
	results.reserve(num_trials);

	const auto start = std::chrono::steady_clock::now();

	for (size_t i{}; i < num_trials; ++i)
	{
		T result = particle_swarm_optimization<T, DIM>(
			num_iters,
			swarm_size,
			inertia,
			acc_coef_1,
			acc_coef_2,
			objective);
		results.push_back(result);
		println("trial {:3}: best value = {}", i + 1, result);
	}

	std::sort(results.begin(), results.end());
	size_t top_n = static_cast<size_t>(num_trials * T{ 0.8 });
	T sum = std::accumulate(results.begin(), results.begin() + top_n, T{ 0 });
	T average_top_80 = sum / top_n;

	println("average of top 80%: {}", average_top_80);

	const auto end = std::chrono::steady_clock::now();

	double total_duration_ms = std::chrono::duration<double, std::milli>(end - start).count();
	println("ca³kowity czas: {}", total_duration_ms);
}