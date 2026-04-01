#pragma once

#include <vector>
#include <cassert>
#include <array>
#include <limits>
#include <algorithm>
#include <numeric>
#include <utility>

#include "random.h"
#include "gtoa_hyperparams.h"
#include "student.h"


namespace gtoa
{

	template <typename T, size_t DIM>
	struct group_teaching_optimization_algorithm
	{
		hyperparams<T, DIM> state;

		T run()
		{
            T min_fitness = std::numeric_limits<T>::max();

            // TODO: Parametrize the outstanding to average ratio
            size_t outstanding_count = state.population_size * state.outstanding_percentage;
            size_t average_count = state.population_size - outstanding_count;

            assert(state.outstanding_percentage <= T{ 1.0 } && state.outstanding_percentage >= T{ 0.0 });

			std::vector<student<T, DIM>> population(state.population_size);
			for (student<T, DIM>& s: population)
			{
				random_vector(s.position,
					state.objective.bounds.first,
					state.objective.bounds.second);
				s.fitness = state.objective.obj_fun(s.position);

				if (s.fitness < min_fitness)
                    min_fitness = s.fitness;
			}

			std::vector<typename std::vector<student<T, DIM>>::iterator>
				leaders(state.n_leaders);

            std::vector<std::array<T, DIM>> pre_teacher_position(population.size());
            std::vector<T> teacher_phase_weight(population.size());
            std::vector<T> teacher_phase_conformism_factor(outstanding_count);
            std::vector<std::array<T, DIM>> teacher_phase_shift(population.size());
            std::vector<T> student_phase_extrapolation_factor(population.size());
            std::vector<T> student_phase_cooperation_factor(population.size());
            std::vector<size_t> student_phase_friend_index(population.size());
            std::vector<std::array<T, DIM>> student_phase_friend_diff(population.size());
            std::vector<std::array<T, DIM>> student_phase_self_diff(population.size());
            std::vector<std::array<T, DIM>> student_phase_shift(population.size());
            
			for (size_t t{}; t < state.num_iterations; ++t)
			{
                // teacher selection
				std::partial_sort(population.begin(),
					population.begin() + leaders.size(), population.end(),
					[](const student<T, DIM>& s1, const student<T, DIM>& s2)
					{
						return s1.fitness < s2.fitness;
					});
                    
				for (size_t i{}; i < state.n_leaders; ++i)
					leaders[i] = population.begin() + i;

                const student<T, DIM>& teacher_candidate_1 = population[0];
                student<T, DIM> teacher_candidate_2 = student<T, DIM> {};

                for(size_t d{}; d < DIM; ++d)
                {
                    // TODO: Implement a weighted sum here
                    T sum{};

                    for(auto leader = population.begin();
                        leader != population.begin() + leaders.size();
                        ++leader)
                    {
                        sum += leader->position[d];
                    }

                    teacher_candidate_2.position[d] = sum / leaders.size();
                }

                teacher_candidate_2.fitness = state.objective.obj_fun(teacher_candidate_2.position);

                student<T, DIM> teacher;
                if(teacher_candidate_1.fitness < teacher_candidate_2.fitness)
                {
                    teacher = teacher_candidate_1;
                } else {
                    teacher = std::move(teacher_candidate_2);
                }
                
                // save the position before the teacher phase for student phase
                for(size_t i{}; i < population.size(); ++i) {
                    pre_teacher_position[i] = population[i].position;
                }

                // teacher phase
				std::nth_element(population.begin(),
                    population.begin() + outstanding_count, population.end(),
                    [](const student<T, DIM>& s1, const student<T, DIM>& s2)
					{
						return s1.fitness < s2.fitness;
					});

                // outstanding students
                for(size_t i{}; i < outstanding_count; i++) {
                    teacher_phase_weight[i] = random_value<T>();
                    teacher_phase_conformism_factor[i] = random_value<T>();
                }
                
                for(size_t d{}; d < DIM; ++d)
                {
                    T sum{};
                    
                    for(auto it = population.begin();
                        it != population.begin() + outstanding_count; ++it)
                    {
                        sum += it->position[d];
                    }
                    
                    T mean_outstanding_position = sum / outstanding_count;

                    for(size_t i = 0; i < outstanding_count; ++i)
                    {
                        T learning_shift = teacher_phase_conformism_factor[i] * mean_outstanding_position
                            + (1 - teacher_phase_conformism_factor[i]) * population[i].position[d];
                        teacher_phase_shift[i][d] = teacher_phase_weight[i]
                            * (teacher.position[d] - state.teaching_factor * learning_shift);
                    }
                }
                
                // average students
                for(size_t i = outstanding_count; i < population.size(); ++i) {
                    teacher_phase_weight[i] = random_value<T>();

                    for(size_t d{}; d < DIM; ++d)
                    {
                        teacher_phase_shift[i][d] = 2 * teacher_phase_weight[i] * (teacher.position[d] - population[i].position[d]);
                    }
                }

                // update student positions after teacher phase
                for(size_t i{}; i < population.size(); ++i) {
                    student<T, DIM> candidate = student<T, DIM> {};

                    for(size_t d{}; d < DIM; ++d) {
                        candidate.position[d] = population[i].position[d] + teacher_phase_shift[i][d];
                    }

                    candidate.fitness = state.objective.obj_fun(candidate.position);

                    if(candidate.fitness < population[i].fitness) {
                        population[i] = std::move(candidate);
                    }
                }

                // student phase
                for(size_t i{}; i < population.size(); ++i) {
                    student_phase_extrapolation_factor[i] = random_value<T>();
                    student_phase_cooperation_factor[i] = random_value<T>();

                    student_phase_friend_index[i] = random_index_other_than(population.size(), i);
                    const student<T, DIM>& friend_student = population[student_phase_friend_index[i]];
                    const student<T, DIM>& self = population[i];

                    for(size_t d{}; d < DIM; ++d) {
                        student_phase_friend_diff[i][d] = self.fitness <= friend_student.fitness
                            ? self.position[d] - friend_student.position[d]
                            : friend_student.position[d] - self.position[d];
                        
                        student_phase_self_diff[i][d] = self.position[d] - pre_teacher_position[i][d];

                        student_phase_shift[i][d] = student_phase_extrapolation_factor[i] * student_phase_self_diff[i][d]
                            + student_phase_cooperation_factor[i] * student_phase_friend_diff[i][d];
                    }
                }

                // update student positions after student phase
                for(size_t i{}; i < population.size(); ++i) {
                    student<T, DIM> candidate = student<T, DIM> {};

                    for(size_t d{}; d < DIM; ++d) {
                        candidate.position[d] = population[i].position[d] + student_phase_shift[i][d];
                    }

                    candidate.fitness = state.objective.obj_fun(candidate.position);

                    if(candidate.fitness < population[i].fitness) {
                        population[i] = std::move(candidate);
                    }

                    // update min_fitness
                    if(population[i].fitness < min_fitness) {
                        min_fitness = population[i].fitness;
                    }
                }
			}
            
			return min_fitness;
		}
	};
    
}