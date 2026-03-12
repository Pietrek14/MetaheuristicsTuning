#pragma once

#include <cmath>
#include <functional>
#include <string>
#include <utility>


namespace gwo::weight_policy
{
	template <typename T>
	using weight_function_t =
		std::function<T(T, std::pair<T, T>)>;

	template <typename T>
	struct weight_policy
	{
		std::string name;
		weight_function_t<T> function;
	};


	template <typename T>
	T constant_weight(T fitness, std::pair<T, T> fitness_bounds)
	{
		return T{ 1 };
	}

	template <typename T>
	T linear_weight(T fitness, std::pair<T, T> fitness_bounds)
	{
		return T{ 1.0 / (fitness - fitness_bounds.first + T{ 1e-6 }) };
	}

	template <typename T>
	T quadratic_weight(T fitness, std::pair<T, T> fitness_bounds)
	{
		return T{ 1.0 / std::pow(fitness - fitness_bounds.first + T{ 1e-6 }, 2) };
	}


	template <typename T>
	inline weight_policy<T> constant
	{
		"constant",
		constant_weight<T>
	};

	template <typename T>
	inline weight_policy<T> linear
	{
		"linear",
		linear_weight<T>
	};

	template <typename T>
	inline weight_policy<T> quadratic
	{
		"quadratic",
		quadratic_weight<T>
	};
}