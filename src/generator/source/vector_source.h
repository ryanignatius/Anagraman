#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "wordsource.h"

class vector_source : public wordsource {
	private:
	std::vector<std::string> source;

	public:
	vector_source();
	vector_source(std::string);
	size_t size();
	std::string get_one(int);
	bool is_existed(std::string);
};