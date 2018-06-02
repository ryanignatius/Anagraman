#pragma once

#include <iostream>

class player {
	private:
	std::string id;
	std::string display_name;
	int score = 0;

	public:
	player();
	player(const std::string&, const std::string&);
	std::string get_id();
	std::string get_display_name();
	int get_score() const;
	void add_score(const int&);
};