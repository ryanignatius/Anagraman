#include "problem.h"

problem::problem(std::string ground) {
	this->ground = ground;
	rescramble();
}

std::string problem::scramble() {
	std::string to_be_scrambled = ground;
	std::random_shuffle(to_be_scrambled.begin(), to_be_scrambled.end());
	return to_be_scrambled;
}

void problem::rescramble() {
	scrambled = scramble();
}

bool problem::check(std::string test) {
	return ground == test;
}

std::string problem::get_ground() {
	return ground;
}

std::string problem::get_scrambled() {
	return scrambled;
}