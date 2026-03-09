#pragma once

#include <execution>
#include <mutex>
#include <vector>
#include <string>

#include "print.h"
#include "timer.h"
#include "statistics.h"
#include "csv_writer.h"
#include "series.h"
#include "objective_factory.h"

template <typename Algorithm, typename Hyperparams>
void run(
	const Hyperparams& hyperparams,
	size_t num_trials,
	float top_percent,
	logging::csv_writer& csv_writer,
	std::mutex& write_mutex)
{
	print_fields(hyperparams.log_values());

	Algorithm algorithm{ hyperparams };

	auto result = statistics(
		std::bind(&Algorithm::run, &algorithm),
		num_trials,
		top_percent
	);

	std::vector<std::string> row;
	for (const auto& field : hyperparams.csv_values())
		row.push_back(field.value);
	row.push_back(std::to_string(result.best_value));
	row.push_back(std::to_string(result.average_value_top_percent));
	row.push_back(std::to_string(result.median_value));
	row.push_back(std::to_string(result.stddev_value));
	row.push_back(std::to_string(result.average_time_ms));

	{
		std::lock_guard lock(write_mutex);
		csv_writer.write_row(row);
	}
}

template <typename Algorithm, typename Hyperparams>
void run_hyperparam_set_parallel(
	const std::vector<Hyperparams>& hyperparams_set,
	size_t num_trials,
	float top_percent,
	logging::csv_writer& csv_writer,
	std::mutex& write_mutex)
{
	std::for_each(
		std::execution::par,
		hyperparams_set.begin(),
		hyperparams_set.end(),
		[&](const Hyperparams& hyperparams)
		{
			run<Algorithm, Hyperparams>(
				hyperparams,
				num_trials,
				top_percent,
				csv_writer,
				write_mutex);
		}
	);
}