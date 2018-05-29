#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "wordsource.h"

class dummysource : public wordsource {
	private:
	std::vector<std::string> source;

	public:
	dummysource();
	size_t size();
	std::string get_one(int);
	bool is_existed(std::string);
};