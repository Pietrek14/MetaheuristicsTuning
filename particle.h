#pragma once

#include <array>
#include <limits>


template <typename T, size_t DIM>
struct particle
{
	std::array<T, DIM> position{};
	std::array<T, DIM> velocity{};
	std::array<T, DIM> best_personal_position{};

	T best_personal_fitness = std::numeric_limits<T>::max();
};