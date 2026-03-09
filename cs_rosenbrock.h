#pragma once

#include <vector>
#include <mutex>
#include <chrono>

#include "objective_factory.h"
#include "visualization.h"

#include "cuckoo_search.h"
#include "cs_hyperparams.h"
#include "cs_generate.h"


namespace cs
{
	inline void run_rosenbrock_experiments()
	{
		using T = float;

		const size_t num_trials = 100;
		const float top_percent = 0.8f;


		std::vector<std::string> headers
			= cs::hyperparams<T, 3>::csv_headers;
		std::vector<std::string> stat_headers = {
			"best_value",
			"average_value_top_80",
			"median_value",
			"stddev_value",
			"average_time_ms"
		};
		headers.insert(headers.end(),
			stat_headers.begin(), stat_headers.end());

		logging::csv_writer writer("cs_results_rosenbrock", headers);
		std::mutex write_mutex;



		auto rosenbrock3 = generate_hyperparams<T, 3>(
			core::make<T, 3>("rosenbrock"), 50, 150);

		auto rosenbrock6 = generate_hyperparams<T, 6>(
			core::make<T, 6>("rosenbrock"), 150, 150);

		auto rosenbrock9 = generate_hyperparams<T, 9>(
			core::make<T, 9>("rosenbrock"), 300, 900);


		const auto start = std::chrono::steady_clock::now();

		run_hyperparam_set_parallel<cs::cuckoo_search<T, 3>,
			cs::hyperparams<T, 3>>(
				rosenbrock3, num_trials, top_percent, writer, write_mutex);
		run_hyperparam_set_parallel<cs::cuckoo_search<T, 6>,
			cs::hyperparams<T, 6>>(
				rosenbrock6, num_trials, top_percent, writer, write_mutex);
		run_hyperparam_set_parallel<cs::cuckoo_search<T, 9>,
			cs::hyperparams<T, 9>>(
				rosenbrock9, num_trials, top_percent, writer, write_mutex);

		const auto end = std::chrono::steady_clock::now();


		double total_duration_ms
			= std::chrono::duration<double, std::milli>(end - start).count();
		println("total_time: {}", total_duration_ms);
	}

}