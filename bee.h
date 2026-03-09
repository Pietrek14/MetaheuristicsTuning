#pragma once

#include <array>
#include <limits>


template <typename T, size_t DIM>
struct bee
{
	std::array<T, DIM> position{};

	T objective_value = std::numeric_limits<T>::max();
	T fitness = std::numeric_limits<T>::max();

	size_t trial{};
};