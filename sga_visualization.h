#pragma once

#include <execution>
#include <mutex>
#include <chrono>
#include <vector>

#include "print.h"
#include "timer.h"
#include "statistics.h"
#include "csv_writer.h"
#include "series.h"

#include "selection.h"
#include "crossover.h"
#include "mutation.h"
#include "simple_genetic_algorithm.h"
#include "sga_hyperparams.h"


template <typename T, size_t DIM>
void run(
	const sga::hyperparams<T, DIM>& hyperparams,
	size_t num_trials,
	float top_percent,
	logging::csv_writer& logger,
	std::mutex& log_mutex)
{
	auto result = statistics(
		simple_genetic_algorithm<T, DIM>,
		num_trials,
		top_percents,
		hyperparams.num_iterations,
		hyperparams.population_size,
		hyperparams.crossover_chance,
		hyperparams.mutation_chance,
		hyperparams.named_selection.function,
		hyperparams.named_crossover.function,
		hyperparams.named_mutation.function,
		hyperparams.objective
	);

	std::lock_guard lock(log_mutex);
	logger.write_row(
		hyperparams.objective.name,
		DIM,
		hyperparams.population_size,
		hyperparams.num_iterations,
		hyperparams.named_selection.name,
		hyperparams.named_crossover.name,
		hyperparams.named_mutation.name,
		hyperparams.crossover_chance,
		hyperparams.mutation_chance,
		result.best_value,
		result.average_value_top_percent,
		result.median_value,
		result.stddev_value,
		result.average_time_ms
	);
}


template <typename T, size_t DIM>
void run_hyperparam_set_parallel(
	const std::vector<sga::hyperparams<T, DIM>>& hyperparams_set,
	size_t num_trials,
	float top_percent,
	logging::csv_writer& logger,
	std::mutex& logger_mutex)
{
	std::for_each(
		std::execution::par,
		hyperparams_set.begin(),
		hyperparams_set.end(),
		[&](const auto& hyperparams)
		{
			println("function: {:>15}, dimensionality: {:2}, sel: {:>15}, cro: {:>15}, mut: {:>15}",
				hyperparams.obj.name,
				DIM,
				hyperparams.named_selection.name,
				hyperparams.named_crossover.name,
				hyperparams.named_mutation.name);

			run(
				hyperparams,
				num_trials,
				top_percent,
				logger,
				logger_mutex);
		});
}


template <typename T, size_t DIM>
std::vector<sga::hyperparams<T, DIM>> generate_hyperparams(
	const core::objective<T, DIM>& obj,
	size_t n_iters,
	size_t population_size)
{
	std::vector<sga::selection::named_selection<T, DIM>> selections =
	{
		{ "roulette wheel", sga::selection::roulette_wheel_selection<T, DIM> },
		{ "tournament", sga::selection::tournament_selector<T, DIM>{ 3 } }
	};

	std::vector<sga::crossover::named_crossover<T, DIM>> crossovers =
	{
		{ "single point", sga::crossover::single_point_crossover<T, DIM> },
		{ "uniform", sga::crossover::uniform_crossover<T, DIM> }
	};

	std::vector<sga::mutation::named_mutation<T, DIM>> mutations =
	{
		{ "bit flip", sga::mutation::bit_flip_mutation<T, DIM> },
		{ "inversion", sga::mutation::inversion_mutation<T, DIM> }
	};

	std::vector<float> crossover_chances = series::linear(0.50f, 1.00f, 51);
	std::vector<float> mutation_chances = series::linear(0.000f, 0.100f, 51);


	std::vector<sga::hyperparams<T, DIM>> hyperparams;
	for (auto& selection : selections)
		for (auto& crossover : crossovers)
			for (auto& mutation : mutations)
				for (float crossover_chance : crossover_chances)
					for (float mutation_chance : mutation_chances)
						hyperparams.push_back({
							obj,
							n_iters,
							population_size,
							selection,
							crossover,
							mutation,
							crossover_chance,
							mutation_chance
							});

	return hyperparams;
}


void visualization()
{
	using T = float;


	const size_t num_trials = 500;
	const float top_percent{ 0.8f };


	std::vector<std::string> headers = {
		"objective_function",
		"dimensionality",
		"population_size",
		"n_iters",
		"selection",
		"crossover",
		"mutation",
		"crossover_chance",
		"mutation_chance",
		"best_value",
		"average_value_top_80",
		"median_value",
		"stddev_value",
		"average_time_ms"
	};

	logging::csv_writer logger("sga_results", headers);

	std::mutex logger_mutex;


	const auto start = std::chrono::steady_clock::now();

	run_hyperparam_set_parallel(
		generate_hyperparams<T, 3>(rastrigin_objective<T, 3>(), 250, 100),
		num_trials,
		top_percent,
		logger,
		logger_mutex);
	run_hyperparam_set_parallel(
		generate_hyperparams<T, 3>(rosenbrock_objective<T, 3>(), 250, 100),
		num_trials,
		top_percent,
		logger,
		logger_mutex);

	const auto end = std::chrono::steady_clock::now();


	double total_duration_ms = std::chrono::duration<double, std::milli>(end - start).count();
	println("ca³kowity czas: {}", total_duration_ms);
}