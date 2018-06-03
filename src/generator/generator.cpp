#include "generator.h"

generator::generator() : rng(time(nullptr)) {
	source = new vector_source("./data/sports.txt");
}

int generator::get_random_index() {
	return rng() % source->size();
}

problem generator::get_problem() {
	return problem(source->get_one(get_random_index()));
}

bool generator::is_existed(std::string test) {
	return source->is_existed(test);
}
