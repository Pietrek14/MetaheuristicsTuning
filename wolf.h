#pragma once

#include <array>
#include <limits>


namespace gwo
{

	template <typename T, size_t DIM>
	struct wolf
	{
		std::array<T, DIM> position{};

		T fitness = std::numeric_limits<T>::max();
	};

}