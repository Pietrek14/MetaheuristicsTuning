#pragma once

#include <array>
#include <limits>


template <typename T, size_t DIM>
struct cuckoo
{
	std::array<T, DIM> position{};
	T fitness = std::numeric_limits<T>::max();
};