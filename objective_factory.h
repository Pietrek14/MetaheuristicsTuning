#pragma once

#include <string>
#include <string_view>
#include <stdexcept>

#include "objective.h"
#include "ackley.h"
#include "griewank.h"
#include "rastrigin.h"
#include "rosenbrock.h"
#include "schwefel.h"
#include "sphere.h"


namespace core
{
	template <typename T, size_t DIM>
	objective<T, DIM> make(std::string_view name)
	{
		if (name == "ackley")
			return ackley_objective<T, DIM>();
		if (name == "griewank")
			return griewank_objective<T, DIM>();
		if (name == "rastrigin")
			return rastrigin_objective<T, DIM>();
		if (name == "rosenbrock")
			return rosenbrock_objective<T, DIM>();
		if (name == "schwefel")
			return schwefel_objective<T, DIM>();
		if (name == "sphere")
			return sphere_objective<T, DIM>();
		
		throw std::invalid_argument("Unknown objective name: " + std::string(name));
	}
}