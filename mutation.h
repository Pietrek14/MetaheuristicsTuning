#pragma once

#include <algorithm>
#include <string>
#include <functional>

#include "random.h"
#include "individual.h"
#include "print.h"
#include "trace_logger.h"


namespace sga::mutation
{
	template <typename T, size_t DIM>
	using mutation_function_t =
		std::function<void(
			chromosome_t<T, DIM>& chromosome,
			float mutation_chance)>;


	template <typename T, size_t DIM>
	struct mutation_operator
	{
		std::string name;
		mutation_function_t<T, DIM> function;
	};


	template <typename T, size_t DIM>
	void bit_flip_mutation(
		chromosome_t<T, DIM>& chromosome,
		float mutation_chance)
	{
		size_t n_flips = random_binomial(CHROMOSOME_SIZE_IN_BITS<T, DIM>, mutation_chance);

		for (size_t i{}; i < n_flips; ++i)
		{
			size_t bit_pos = random_index<CHROMOSOME_SIZE_IN_BITS<T, DIM>>();
			size_t gene_index = bit_pos / ELEMENT_SIZE_IN_BITS<T, DIM>;
			size_t bit_index = bit_pos % ELEMENT_SIZE_IN_BITS<T, DIM>;

			chromosome[gene_index] ^= chromosome_element_t<T, DIM>(1) << bit_index;
		}
	}


	template <typename T, size_t DIM>
	void inversion_mutation(
		chromosome_t<T, DIM>& chromosome,
		float mutation_chance)
	{
		if (random_value<float>() > mutation_chance)
			return;

		size_t gene_index = random_index<DIM>();
		chromosome_element_t<T, DIM>& gene = chromosome[gene_index];

		size_t i = random_index<ELEMENT_SIZE_IN_BITS<T, DIM>>();
		size_t j = random_index<ELEMENT_SIZE_IN_BITS<T, DIM>>();
		if (i > j)
			std::swap(i, j);

		while (i < j)
		{
			bool bi = (gene >> i) & 1;
			bool bj = (gene >> j) & 1;

			if (bi != bj)
			{
				gene ^= chromosome_element_t<T, DIM>(1) << i;
				gene ^= chromosome_element_t<T, DIM>(1) << j;
			}

			++i;
			--j;
		}
	}
}