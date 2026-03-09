#pragma once

#include <random>
#include <array>
#include <numbers>
#include <concepts>
#include <numeric>
#include <algorithm>


inline thread_local std::mt19937 rng(std::random_device{}());


template <std::floating_point T>
T random_value()
{
	static thread_local std::uniform_real_distribution<T> dist(T{ 0 }, T{ 1 });

	return dist(rng);
}


template <std::floating_point T>
T random_value(T lower_bound, T upper_bound)
{
	const T scale = (upper_bound - lower_bound) / T(rng.max());
	return lower_bound + scale * rng();
}


template <size_t N>
size_t random_index()
{
	static thread_local std::uniform_int_distribution<size_t> dist(0, N - 1);

	return dist(rng);
}

inline size_t random_index(size_t N)
{
	std::uniform_int_distribution<size_t> dist(0, N - 1);

	return dist(rng);
}

inline size_t random_index_other_than(size_t N, size_t k)
{
	size_t i;
	do
		i = random_index(N);
	while (i == k);

	return i;
}


template <size_t MIN, size_t MAX>
size_t random_index_range()
{
	static thread_local std::uniform_int_distribution<size_t> dist(MIN, MAX - 1);

	return dist(rng);
}


template <std::floating_point T, size_t DIM>
void random_vector(
	std::array<T, DIM>& x,
	T lower_bound,
	T upper_bound)
{
	static thread_local std::uniform_real_distribution<T> dist(T{ 0 }, T{ 1 });

	T range = upper_bound - lower_bound;
	for (T& xi: x)
		xi = lower_bound + range * dist(rng);
}


template <std::unsigned_integral T, size_t DIM>
void random_bits(std::array<T, DIM>& x)
{
	static thread_local std::uniform_int_distribution<T> dist(
		std::numeric_limits<T>::min(),
		std::numeric_limits<T>::max());

	for (T& xi : x)
		xi = dist(rng);
}


template <std::unsigned_integral T>
void random_bits(T& x)
{
	static thread_local std::uniform_int_distribution<T> dist(
		std::numeric_limits<T>::min(),
		std::numeric_limits<T>::max());

	x = dist(rng);
}


template <std::unsigned_integral T, std::floating_point U>
T random_binomial(T trials, U probability)
{
	static thread_local T last_trials{};
	static thread_local U last_probability{};
	static thread_local std::binomial_distribution<T> dist{};

	if (trials != last_trials || probability != last_probability)
	{
		dist = std::binomial_distribution<T>(trials, probability);
		last_trials = trials;
		last_probability = probability;
	}
	
	return dist(rng);
}

template <std::floating_point T>
T levy()
{
	static thread_local std::normal_distribution<T> normal_dist(T{ 0 }, T{ 1 });
	static constexpr T beta{ 1.5 };
	static constexpr T beta_inv = T{ 1 } / beta;
	static const T sigma = std::pow(
		std::tgamma(T{ 1 } + beta) * std::sin(std::numbers::pi_v<T> *beta / T{ 2 }) /
		(std::tgamma((T{ 1 } + beta) / 2) * beta * std::pow(T{ 2 }, (beta - T{ 1 }) / T{ 2 })),
		T{ 1 } / beta
	);

	T u = normal_dist(rng) * sigma;
	T v = normal_dist(rng);

	return u / std::exp(beta_inv * std::log(std::abs(v)));
}

template <std::floating_point T>
T normal()
{
	static thread_local std::normal_distribution<T> dist(T{ 0 }, T{ 1 });

	return dist(rng);
}

template <std::floating_point T, size_t DIM>
void tent_vector(
	std::array<T, DIM>& x,
	T lower_bound,
	T upper_bound,
	size_t population_size)
{
	static thread_local std::array<T, DIM> state;
	static thread_local bool initialized = false;

	if (!initialized)
	{
		for (T& xi: state)
			xi = random_value<T>();
		initialized = true;
	}

	for (size_t d{}; d < DIM; ++d)
		x[d] = tent_value<T>(state[d],
			population_size,
			lower_bound,
			upper_bound);
}

template <std::floating_point T>
T tent_value(
	T& state,
	size_t population_size)
{
	if (state < T{ 0.5 })
	{
		state = T{ 2 } *state
			+ random_value<T>()
			/ static_cast<T>(population_size);
	}
	else
	{
		state = T{ 2 } *(T{ 1 } - state)
			+ random_value<T>()
			/ static_cast<T>(population_size);
	}

	state = std::clamp(state,
		T{ 1e-12 }, T{ 1 } - T{ 1e-12 });

	return state;
}

template <std::floating_point T>
T tent_value(
	T& state,
	size_t population_size,
	T lower_bound,
	T upper_bound)
{
	return lower_bound
		+ tent_value<T>(state, population_size)
		* (upper_bound - lower_bound);
}


inline std::vector<size_t> random_subset(size_t n, size_t k)
{
	std::vector<size_t> indices(n);
	std::iota(indices.begin(), indices.end(), 0);

	std::shuffle(indices.begin(), indices.end(), rng);
	indices.resize(k);

	return indices;
}