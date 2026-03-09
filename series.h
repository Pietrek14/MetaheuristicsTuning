#pragma once

#include <vector>
#include <cmath>


namespace series
{
	template <typename T>
	std::vector<T> linear(T min_val, T max_val, size_t steps)
	{
		std::vector<T> values;
		values.reserve(steps);

		T step = (max_val - min_val) / static_cast<T>(steps - 1);

		for (size_t i = 0; i < steps; ++i)
			values.push_back(min_val + step * static_cast<T>(i));

		return values;
	}


	template <typename T>
	std::vector<T> exponential(T min_val, T max_val, size_t steps)
	{
		std::vector<T> values;
		values.reserve(steps);

		T log_min = std::log(min_val);
		T log_max = std::log(max_val);
		T step = (log_max - log_min) / static_cast<T>(steps - 1);

		for (size_t i = 0; i < steps; ++i)
			values.push_back(std::exp(log_min + step * static_cast<T>(i)));

		return values;
	}
}