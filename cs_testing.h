#pragma once

#include "print.h"
#include "timer.h"
#include "statistics.h"
#include "cuckoo_search.h"
#include "objective_factory.h"

#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>


void objective_testing()
{
	using T = double;
	constexpr size_t DIM = 6;


	T alpha{ 0.05 };
	T gamma{ 0.1 };
	T pa{ 0.3 };
	size_t population_size = 120;
	size_t num_iters = 300;
	auto objective = core::make<T, DIM>("rastrigin");


	const size_t num_trials = 500;

	std::vector<T> results;
	results.reserve(num_trials);

	const auto start = std::chrono::steady_clock::now();

	for (size_t i{}; i < num_trials; ++i)
	{
		T result = cuckoo_search<T, DIM>(
			num_iters,
			population_size,
			alpha,
			gamma,
			pa,
			objective);
		results.push_back(result);
		println("trial {:3}: best value = {}", i + 1, result);
	}

	const auto end = std::chrono::steady_clock::now();
	double total_duration_ms =
		std::chrono::duration<double, std::milli>(end - start).count();

	std::sort(results.begin(), results.end());
	size_t top_n = static_cast<size_t>(num_trials * T{ 0.8 });
	T sum = std::accumulate(results.begin(), results.begin() + top_n, T{ 0 });
	T average_top_80 = sum / top_n;

	println("average of top 80%: {}", average_top_80);
	println("total time: {}", total_duration_ms);
}