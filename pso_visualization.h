#pragma once

#include <execution>
#include <mutex>
#include <chrono>
#include <vector>
#include <thread>
#include <string>

#include "print.h"
#include "timer.h"
#include "statistics.h"
#include "particle_swarm_optimization.h"
#include "csv_writer.h"
#include "series.h"
#include "pso_hyperparams.h"
#include "objective_factory.h"

using namespace std::literals;


namespace pso
{

	template <typename T, size_t DIM>
	void run(
		const pso::hyperparams<T, DIM>& hyperparams,
		size_t num_trials,
		float top_percent,
		logging::csv_writer& csv_writer,
		std::mutex& write_mutex)
	{
		println("objective: {:>15}, dimensionality: {:2}, inertia: {:4.1f}, "s
			+ "acc1: {:5.2f}, acc2: {:5.2f}"s,
			hyperparams.obj.name,
			DIM,
			hyperparams.inertia,
			hyperparams.acc_coef_1,
			hyperparams.acc_coef_2);

		auto result = statistics(
			particle_swarm_optimization<T, DIM>,
			num_trials,
			top_percent,
			hyperparams.n_iters,
			hyperparams.swarm_size,
			hyperparams.inertia,
			hyperparams.acc_coef_1,
			hyperparams.acc_coef_2,
			hyperparams.obj
		);

		std::lock_guard lock(write_mutex);
		csv_writer.write_row(
			hyperparams.inertia,
			hyperparams.acc_coef_1,
			hyperparams.acc_coef_2,
			hyperparams.swarm_size,
			hyperparams.n_iters,
			hyperparams.obj.name,
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
		const std::vector<pso::hyperparams<T, DIM>>& hyperparams_set,
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
	std::vector<pso::hyperparams<T, DIM>> generate_hyperparams(
		const core::objective<T, DIM>& obj,
		size_t num_iters,
		size_t swarm_size)
	{
		std::vector<T> inertias = series::linear(T{ 0.4 }, T{ 0.8 }, 5);
		std::vector<T> acc_coefs_1 = series::linear(T{ 0.15 }, T{ 3.0 }, 20);
		std::vector<T> acc_coefs_2 = series::linear(T{ 0.15 }, T{ 3.0 }, 20);

		std::vector<pso::hyperparams<T, DIM>> hyperparams;

		for (T inertia : inertias)
			for (T acc_coef_1 : acc_coefs_1)
				for (T acc_coef_2 : acc_coefs_2)
					hyperparams.push_back({
						obj,
						num_iters,
						swarm_size,
						inertia,
						acc_coef_1,
						acc_coef_2
						});

		return hyperparams;
	}


	void visualization()
	{
		using T = float;


		const size_t num_trials = 20;
		const float top_percent = 0.8f;


		std::vector<std::string> headers = {
			"inertia",
			"acc_coef_1",
			"acc_coef_2",
			"swarm_size",
			"num_iters",
			"objective",
			"dimensionality",
			"best_value",
			"average_value_top_80",
			"median_value",
			"stddev_value",
			"average_time_ms"
		};

		logging::csv_writer writer("pso_results", headers);

		std::mutex write_mutex;


		const auto start = std::chrono::steady_clock::now();

		run_hyperparam_set_parallel<T, 3>(
			generate_hyperparams<T, 3>(core::make<T, 3>(
				"rastrigin"), 50, 150),
			num_trials,
			top_percent,
			writer,
			write_mutex);
		run_hyperparam_set_parallel<T, 6>(
			generate_hyperparams<T, 6>(core::make<T, 6>(
				"rastrigin"), 150, 450),
			num_trials,
			top_percent,
			writer,
			write_mutex);
		run_hyperparam_set_parallel<T, 9>(
			generate_hyperparams<T, 9>(core::make<T, 9>(
				"rastrigin"), 300, 600),
			num_trials,
			top_percent,
			writer,
			write_mutex);
		run_hyperparam_set_parallel<T, 3>(
			generate_hyperparams<T, 3>(core::make<T, 3>(
				"rosenbrock"), 50, 150),
			num_trials,
			top_percent,
			writer,
			write_mutex);
		run_hyperparam_set_parallel<T, 6>(
			generate_hyperparams<T, 6>(core::make<T, 6>(
				"rosenbrock"), 150, 450),
			num_trials,
			top_percent,
			writer,
			write_mutex);
		run_hyperparam_set_parallel<T, 9>(
			generate_hyperparams<T, 9>(core::make<T, 9>(
				"rosenbrock"), 300, 900),
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