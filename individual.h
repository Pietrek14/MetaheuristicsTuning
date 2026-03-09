#pragma once

#include <array>
#include <functional>
#include <type_traits>
#include <limits>

#include "objective.h"
#include "random.h"


template <typename T, size_t DIM>
class individual
{
public:

	using objective_function = std::function<T(const std::array<T, DIM>&)>;

	using chromosome_element_t =
		std::conditional_t<sizeof(T) == 4, uint32_t,
		std::conditional_t<sizeof(T) == 8, uint64_t,
		void>>;

	static_assert(!std::is_same_v<chromosome_element_t, void>,
		"Unsupported T size");

	using chromosome_t = std::array<chromosome_element_t, DIM>;


	individual(const core::objective<T, DIM>& obj) :
		bounds_(obj.bounds),
		obj_fun_(obj.obj_fun),
		chromosome_{},
		phenotype_{}
	{
		random_bits(chromosome_);
		decode();
	}

	const std::array<T, DIM>& phenotype() const noexcept
	{
		return phenotype_;
	}

	chromosome_t& chromosome() noexcept
	{
		return chromosome_;
	}

	T fitness() const noexcept
	{
		return fitness_;
	}

	void decode() noexcept
	{
		for (size_t i{}; i < DIM; ++i)
		{
			phenotype_[i] = bounds_.first +
				(static_cast<T>(chromosome_[i]) /
					static_cast<T>(std::numeric_limits<chromosome_element_t>::max())) *
				(bounds_.second - bounds_.first);
		}
		fitness_ = obj_fun_(phenotype_);
	}


private:

	std::pair<T, T> bounds_;

	objective_function obj_fun_;

	chromosome_t chromosome_;
	std::array<T, DIM> phenotype_;

	T fitness_{};
};


template <typename T, size_t DIM>
using chromosome_t = typename individual<T, DIM>::chromosome_t;

template <typename T, size_t DIM>
using chromosome_element_t = typename individual<T, DIM>::chromosome_element_t;

template <typename T, size_t DIM>
constexpr size_t ELEMENT_SIZE_IN_BITS = sizeof(chromosome_element_t<T, DIM>) * 8;

template <typename T, size_t DIM>
constexpr size_t CHROMOSOME_SIZE_IN_BITS = ELEMENT_SIZE_IN_BITS<T, DIM> * DIM;