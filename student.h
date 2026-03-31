#pragma once

#include <array>
#include <limits>


namespace gtoa
{

	template <typename T, size_t DIM>
	struct student
	{
		std::array<T, DIM> position{};

		T fitness = std::numeric_limits<T>::max();
	};

}