#pragma once

#include <execution>
#include <mutex>
#include <chrono>
#include <vector>
#include <string>

#include "print.h"
#include "timer.h"
#include "statistics.h"
#include "csv_writer.h"
#include "series.h"
#include "objective_factory.h"

#include "grey_wolf_optimization.h"
#include "gwo_hyperparams.h"

using namespace std::literals;


namespace gwo
{

	template <typename T, size_t DIM>
	void run(
		const gwo::hyperparams<T, DIM>& hyperparams,
		size_t num_trials,
		float top_percent,
		logging::csv_writer& csv_writer,
		std::mutex& write_mutex)
	{
		println(("objective: {:>15}, dimensionality: {:2}, n_leaders: {:2} "s
			+ ", a_factor: {:5.2f}, c_factor: {:5.2f}"s),
			hyperparams.objective.name,
			DIM,
			hyperparams.n_leaders,
			hyperparams.a_factor,
			hyperparams.c_factor);

		auto result = statistics(
			grey_wolf_optimization<T, DIM>,
			num_trials,
			top_percent,
			hyperparams.num_iterations,
			hyperparams.population_size,
			hyperparams.n_leaders,
			hyperparams.a_factor,
			hyperparams.c_factor,
			hyperparams.objective
		);

		std::lock_guard lock(write_mutex);
		csv_writer.write_row(
			hyperparams.n_leaders,
			hyperparams.a_factor,
			hyperparams.c_factor,
			hyperparams.population_size,
			hyperparams.num_iterations,
			hyperparams.objective.name,
			DIM,
			result.best_value,
			result.average_value_top_percent,
			result.median_value,
			result.stddev_value,
			result.average_time_ms
		);
	}


	template <typename T, size_t DIM>
	void run_hyperparam_set_parallel(
		const std::vector<gwo::hyperparams<T, DIM>>& hyperparams_set,
		size_t num_trials,
		float top_percent,
		logging::csv_writer& csv_writer,
		std::mutex& write_mutex)
	{
		std::for_each(
			std::execution::par,
			hyperparams_set.begin(),
			hyperparams_set.end(),
			[&](const auto& hyperparams)
			{
				run(
					hyperparams,
					num_trials,
					top_percent,
					csv_writer,
					write_mutex);
			}
		);
	}


	template <typename T, size_t DIM>
	std::vector<gwo::hyperparams<T, DIM>> generate_hyperparams(
		const core::objective<T, DIM>& obj,
		size_t num_iters,
		size_t swarm_size)
	{
		std::vector<size_t> n_leaderss = series::linear(size_t{ 3 }, size_t{ 11 }, 5);
		std::vector<T> a_factors = series::linear(T{ 1 }, T{ 20 }, 20);
		std::vector<T> c_factors = series::linear(T{ 1 }, T{ 20 }, 20);

		std::vector<gwo::hyperparams<T, DIM>> hyperparams;

		for (size_t n_leaders : n_leaderss)
			for (T a_factor : a_factors)
				for (T c_factor : c_factors)
					hyperparams.push_back({
						obj,
						num_iters,
						swarm_size,
						n_leaders,
						a_factor,
						c_factor,
						});

		return hyperparams;
	}


	void visualization()
	{
		using T = float;


		const size_t num_trials = 20;
		const float top_percent = 0.8f;


		std::vector<std::string> headers = {
			"n_leaders",
			"a_factor",
			"c_factor",
			"population_size",
			"num_iters",
			"objective",
			"dimensionality",
			"best_value",
			"average_value_top_80",
			"median_value",
			"stddev_value",
			"average_time_ms"
		};

		logging::csv_writer writer("gwo_results", headers);

		std::mutex write_mutex;


		const auto start = std::chrono::steady_clock::now();

		run_hyperparam_set_parallel<T, 3>(
			generate_hyperparams<T, 3>(
				core::make<T, 3>("rastrigin"), 50, 150),
			num_trials,
			top_percent,
			writer,
			write_mutex);
		run_hyperparam_set_parallel<T, 6>(
			generate_hyperparams<T, 6>(
				core::make<T, 6>("rastrigin"), 150, 450),
			num_trials,
			top_percent,
			writer,
			write_mutex);
		run_hyperparam_set_parallel<T, 9>(
			generate_hyperparams<T, 9>(
				core::make<T, 9>("rastrigin"), 300, 900),
			num_trials,
			top_percent,
			writer,
			write_mutex);
		run_hyperparam_set_parallel<T, 3>(
			generate_hyperparams<T, 3>(
				core::make<T, 3>("rosenbrock"), 50, 150),
			num_trials,
			top_percent,
			writer,
			write_mutex);
		run_hyperparam_set_parallel<T, 6>(
			generate_hyperparams<T, 6>(
				core::make<T, 6>("rosenbrock"), 150, 450),
			num_trials,
			top_percent,
			writer,
			write_mutex);
		run_hyperparam_set_parallel<T, 9>(
			generate_hyperparams<T, 9>(
				core::make<T, 9>("rosenbrock"), 300, 900),
			num_trials,
			top_percent,
			writer,
			write_mutex);

		const auto end = std::chrono::steady_clock::now();


		double total_duration_ms
			= std::chrono::duration<double, std::milli>(end - start).count();
		println("ca³kowity czas: {}", total_duration_ms);
	}

}