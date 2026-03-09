#pragma once

#include <vector>
#include <mutex>
#include <chrono>

#include "series.h"
#include "objective_factory.h"
#include "visualization.h"

#include "cuckoo_search.h"
#include "cs_hyperparams.h"


namespace cs
{

	template <typename T, size_t DIM>
	std::vector<cs::hyperparams<T, DIM>> generate_hyperparams(
		const core::objective<T, DIM>& obj,
		size_t num_iters,
		size_t swarm_size)
	{
		std::vector<T> pa0s = series::linear(T{ 0.05 }, T{ 0.25 }, 5);
		std::vector<T> alpha0s = series::exponential(T{ 0.1 }, T{ 3.00 }, 20);
		std::vector<T> gammas = series::exponential(T{ 0.1 }, T{ 3.00 }, 20);

		std::vector<cs::hyperparams<T, DIM>> hyperparams;

		for (T pa0 : pa0s)
			for (T alpha0 : alpha0s)
				for (T gamma : gammas)
					hyperparams.push_back({
						obj,
						num_iters,
						swarm_size,
						pa0,
						alpha0,
						gamma
						});

		return hyperparams;
	}



	inline void cs_visualization()
	{
		using T = float;

		const size_t num_trials = 20;
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

		logging::csv_writer writer("cs_results", headers);
		std::mutex write_mutex;



		auto rastrigin3 = generate_hyperparams<T, 3>(
			core::make<T, 3>("rastrigin"), 50, 150);

		auto rastrigin6 = generate_hyperparams<T, 6>(
			core::make<T, 6>("rastrigin"), 150, 450);

		auto rastrigin9 = generate_hyperparams<T, 9>(
			core::make<T, 9>("rastrigin"), 300, 900);

		auto rosenbrock3 = generate_hyperparams<T, 3>(
			core::make<T, 3>("rosenbrock"), 50, 150);

		auto rosenbrock6 = generate_hyperparams<T, 6>(
			core::make<T, 6>("rosenbrock"), 150, 450);

		auto rosenbrock9 = generate_hyperparams<T, 9>(
			core::make<T, 9>("rosenbrock"), 300, 900);


		const auto start = std::chrono::steady_clock::now();

		run_hyperparam_set_parallel<cs::cuckoo_search<T, 3>,
			cs::hyperparams<T, 3>>(
				rastrigin3, num_trials, top_percent, writer, write_mutex);
		run_hyperparam_set_parallel<cs::cuckoo_search<T, 6>,
			cs::hyperparams<T, 6>>(
				rastrigin6, num_trials, top_percent, writer, write_mutex);
		run_hyperparam_set_parallel<cs::cuckoo_search<T, 9>,
			cs::hyperparams<T, 9>>(
				rastrigin9, num_trials, top_percent, writer, write_mutex);

		run_hyperparam_set_parallel<cs::cuckoo_search<T, 3>,
			cs::hyperparams<T, 3>>(
				rosenbrock3, num_trials, top_percent, writer, write_mutex);
		run_hyperparam_set_parallel < cs::cuckoo_search<T, 6>,
			cs::hyperparams<T, 6>>(
				rosenbrock6, num_trials, top_percent, writer, write_mutex);
		run_hyperparam_set_parallel < cs::cuckoo_search<T, 9>,
			cs::hyperparams<T, 9>>(
				rosenbrock9, num_trials, top_percent, writer, write_mutex);

		const auto end = std::chrono::steady_clock::now();


		double total_duration_ms
			= std::chrono::duration<double, std::milli>(end - start).count();
		println("ca³kowity czas: {}", total_duration_ms);
	}

}