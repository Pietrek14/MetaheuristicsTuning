#pragma once

#include <functional>
#include <cmath>
#include <string>


namespace gwo::policy
{
	template <typename T>
	using convergence_function_t =
		std::function<T(size_t, size_t)>;

	template <typename T>
	struct convergence_policy
	{
		std::string name;
		convergence_function_t<T> function;
	};


	template <typename T>
	T linear_convergence(size_t t, size_t max_iterations)
	{
		return (1 - static_cast<T>(t) / max_iterations);
	}

	template <typename T>
	T quadratic_convergence(size_t t, size_t max_iterations)
	{
		return (1 - std::pow(static_cast<T>(t) / max_iterations, 2));
	}

	template <typename T>
	struct exponential_convergence
	{
		explicit exponential_convergence(T factor)
			: factor(factor) {}

		T operator()(size_t t, size_t max_iterations) const
		{
			return std::exp(-factor * static_cast<T>(t) / max_iterations);
		}

	private:
		T factor;
	};


	template <typename T>
	inline convergence_policy<T> linear
	{
		"linear",
		linear_convergence<T>
	};

	template <typename T>
	inline convergence_policy<T> quadratic
	{
		"quadratic",
		quadratic_convergence<T>
	};

	template <typename T>
	inline convergence_policy<T> exponential_3
	{
		"exponential_3",
		exponential_convergence<T>{ 3.0 }
	};

	template <typename T>
	inline convergence_policy<T> exponential_5
	{
		"exponential_5",
		exponential_convergence<T>{ 5.0 }
	};
}