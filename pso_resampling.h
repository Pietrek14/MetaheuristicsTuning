#pragma once

#include <vector>
#include <mutex>
#include <string>
#include <execution>
#include <type_traits>

#include "print.h"
#include "particle_swarm_optimization.h"
#include "statistics.h"
#include "csv_writer.h"
#include "csv_reader.h"
#include "objective.h"
#include "pso_hyperparams.h"
#include "objective_factory.h"


template <typename T>
T parse(const std::string& s)
{
	if constexpr (std::is_same_v<T, float>)
		return std::stof(s);
	else if constexpr (std::is_same_v<T, double>)
		return std::stod(s);
	else if constexpr (std::is_same_v<T, long double>)
		return std::stold(s);
	else
		static_assert(!sizeof(T), "Unsupported type for parsing.");
}


template <typename T, size_t DIM>
void run_pso(
	const pso::hyperparams<T, DIM>& hyperparams,
	size_t n_trials,
	float top_percent,
	logging::csv_writer& writer,
	std::mutex& write_mutex)
{
	println("objective: {:>15}, dimensionality: {:2}, inertia: {:4.1f}, acc1: {:5.2f}, acc2: {:5.2f}",
		hyperparams.obj.name,
		DIM,
		hyperparams.inertia,
		hyperparams.acc_coef_1,
		hyperparams.acc_coef_2);

	auto result = statistics(
		particle_swarm_optimization<T, DIM>,
		n_trials,
		top_percent,
		hyperparams.n_iters,
		hyperparams.swarm_size,
		hyperparams.inertia,
		hyperparams.acc_coef_1,
		hyperparams.acc_coef_2,
		hyperparams.obj
	);

	std::lock_guard lock(write_mutex);

	writer.write_row(
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
	size_t n_trials,
	float top_percent,
	logging::csv_writer& writer,
	std::mutex& write_mutex)
{
	std::for_each(
		std::execution::par,
		hyperparams_set.begin(),
		hyperparams_set.end(),
		[&](const auto& hyperparams)
		{
			run_pso(
				hyperparams,
				n_trials,
				top_percent,
				writer,
				write_mutex);
		});
}


void pso_resampling()
{
	using T = float;


	const size_t n_trials = 5000;
	const float top_percent = 0.8f;


	logging::csv_reader reader("pso_results_noise.csv");

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

	logging::csv_writer writer("pso_resampling_results", headers);

	std::mutex write_mutex;


	std::vector<pso::hyperparams<T, 3>> objective_3;
	std::vector<pso::hyperparams<T, 6>> objective_6;
	std::vector<pso::hyperparams<T, 9>> objective_9;


	std::vector<std::string> row;
	while (reader.read_row(row))
	{
		const T inertia = parse<T>(row[0]);
		const T acc_coef_1 = parse<T>(row[1]);
		const T acc_coef_2 = parse<T>(row[2]);
		const size_t swarm_size = static_cast<size_t>(std::stoul(row[3]));
		const size_t n_iters = static_cast<size_t>(std::stoul(row[4]));
		const std::string objective_name = row[5];
		const size_t dimensionality = static_cast<size_t>(std::stoul(row[6]));

		if (dimensionality == 3)
			objective_3.push_back({
				core::make<T, 3>(objective_name),
				n_iters,
				swarm_size,
				inertia,
				acc_coef_1,
				acc_coef_2
				});
		if (dimensionality == 6)
			objective_6.push_back({
				core::make<T, 6>(objective_name),
				n_iters,
				swarm_size,
				inertia,
				acc_coef_1,
				acc_coef_2
				});
		if (dimensionality == 9)
			objective_9.push_back({
				core::make<T, 9>(objective_name),
				n_iters,
				swarm_size,
				inertia,
				acc_coef_1,
				acc_coef_2
				});
	}


	run_hyperparam_set_parallel<T, 3>(
		objective_3,
		n_trials,
		top_percent,
		writer,
		write_mutex);
	run_hyperparam_set_parallel<T, 6>(
		objective_6,
		n_trials,
		top_percent,
		writer,
		write_mutex);
	run_hyperparam_set_parallel<T, 9>(
		objective_9,
		n_trials,
		top_percent,
		writer,
		write_mutex);
}