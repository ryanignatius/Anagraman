#pragma once
#include <iostream>
#include <algorithm>

class problem {
	private:
	std::string ground;
	std::string scrambled;
	std::string scramble();

	public:
	problem(std::string);
	void rescramble();
	bool check(std::string);
	std::string get_scrambled();
};