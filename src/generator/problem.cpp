#include "problem.h"

problem::problem() {}

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

bool alphanum(const char& c) {
	return !(c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9');
}

bool problem::check(std::string test) {
	std::string cground = ground;
	cground.erase(std::remove_if(cground.begin(), cground.end(), alphanum), cground.end());
	test.erase(std::remove_if(test.begin(), test.end(), alphanum), test.end());
	return cground == test;
}

std::string problem::get_ground() {
	return ground;
}

std::string problem::get_scrambled() {
	return scrambled;
}
