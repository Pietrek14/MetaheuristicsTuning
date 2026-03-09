#pragma once

#include <vector>
#include <mutex>
#include <chrono>

#include "objective_factory.h"
#include "visualization.h"

#include "artificial_bee_colony.h"
#include "abc_hyperparams.h"
#include "abc_generate.h"


namespace abc
{
	inline void run_rastrigin_experiments()
	{
		using T = float;

		const size_t num_trials = 100;
		const float top_percent = 0.8f;


		std::vector<std::string> headers
			= hyperparams<T, 3>::csv_headers;
		std::vector<std::string> stat_headers = {
			"best_value",
			"average_value_top_80",
			"median_value",
			"stddev_value",
			"average_time_ms"
		};
		headers.insert(headers.end(),
			stat_headers.begin(), stat_headers.end());

		logging::csv_writer writer("abc_results_rastrigin", headers);
		std::mutex write_mutex;


		auto rastrigin3 = generate_hyperparams<T, 3>(
			core::make<T, 3>("rastrigin"), 50, 75);

		auto rastrigin6 = generate_hyperparams<T, 6>(
			core::make<T, 6>("rastrigin"), 150, 225);

		auto rastrigin9 = generate_hyperparams<T, 9>(
			core::make<T, 9>("rastrigin"), 300, 450);


		const auto start = std::chrono::steady_clock::now();

		run_hyperparam_set_parallel<
			artificial_bee_colony<T, 3>, hyperparams<T, 3>>(
				rastrigin3, num_trials, top_percent, writer, write_mutex);
		run_hyperparam_set_parallel<
			artificial_bee_colony<T, 6>, hyperparams<T, 6>>(
				rastrigin6, num_trials, top_percent, writer, write_mutex);
		run_hyperparam_set_parallel<
			artificial_bee_colony<T, 9>, hyperparams<T, 9>>(
				rastrigin9, num_trials, top_percent, writer, write_mutex);

		const auto end = std::chrono::steady_clock::now();


		double total_duration_ms
			= std::chrono::duration<double, std::milli>(end - start).count();
		println("total time: {}", total_duration_ms);
	}

}