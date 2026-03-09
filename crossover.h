#pragma once

#include <algorithm>
#include <string>
#include <functional>

#include "random.h"
#include "individual.h"
#include "print.h"
#include "trace_logger.h"


namespace sga::crossover
{
	template <typename T, size_t DIM>
	using crossover_function_t = 
		std::function<void(
			chromosome_t<T, DIM>& parent1,
			chromosome_t<T, DIM>& parent2,
			float crossover_chance)>;

	template<typename T, size_t DIM>
	struct crossover_operator
	{
		std::string name;
		crossover_function_t<T, DIM> function;
	};;


	template <typename T, size_t DIM>
	void single_point_crossover(
		chromosome_t<T, DIM>& parent1,
		chromosome_t<T, DIM>& parent2,
		float crossover_chance)
	{
		if (random_value<float>() > crossover_chance)
			return;

		size_t element_index = random_index<DIM>();

		chromosome_element_t<T, DIM>& a = parent1[element_index];
		chromosome_element_t<T, DIM>& b = parent2[element_index];

		size_t bit_index = random_index<ELEMENT_SIZE_IN_BITS<T, DIM>>();

		chromosome_element_t<T, DIM> mask = (chromosome_element_t<T, DIM>(1) << bit_index) - 1;

		chromosome_element_t<T, DIM> a_bits = a & mask;
		chromosome_element_t<T, DIM> b_bits = b & mask;

		a = (a & ~mask) | b_bits;
		b = (b & ~mask) | a_bits;

		if (element_index + 1 < DIM)
			for (size_t i = element_index + 1; i < DIM; ++i)
				std::swap(parent1[i], parent2[i]);
	}


	template <typename T, size_t DIM>
	void uniform_crossover(
		chromosome_t<T, DIM>& parent1,
		chromosome_t<T, DIM>& parent2,
		float crossover_chance)
	{
		if (random_value<float>() > crossover_chance)
			return;

		chromosome_element_t<T, DIM> mask{};
		chromosome_element_t<T, DIM> mask2{};

		for (size_t i{}; i < DIM; ++i)
		{
			chromosome_element_t<T, DIM>& a = parent1[i];
			chromosome_element_t<T, DIM>& b = parent2[i];

			random_bits(mask);
			random_bits(mask2);
			mask &= mask2;

			chromosome_element_t<T, DIM> a_bits = a & mask;
			chromosome_element_t<T, DIM> b_bits = b & mask;

			a = (a & ~mask) | b_bits;
			b = (b & ~mask) | a_bits;
		}
	}
}