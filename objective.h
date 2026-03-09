#pragma once

#include <utility>
#include <functional>
#include <array>
#include <string>


namespace core
{
	template <typename T, size_t DIM>
	struct objective
	{
		static constexpr size_t dimensionality = DIM;

		std::pair<T, T> bounds{};
		std::function<T(const std::array<T, DIM>&)> obj_fun{};
		std::string name{};
	};
}