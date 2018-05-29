#pragma once
#include "source/vector_source.h"
#include "problem.h"
#include <random>

class generator {
	private:
	std::mt19937 rng;
	wordsource* source;
	int get_random_index();

	public:
	generator();
	problem get_problem();
	bool is_existed(std::string);
};