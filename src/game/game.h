#pragma once

#include <vector>
#include <unordered_map>

#include "player.h"
#include "../generator/generator.h"

class game {
	private:
	std::unordered_map<std::string, player> players;
	problem current_problem;
	generator mgenerator;
	int score_increment = 1;
	void increment_score(const std::string&, const int&);

	public:
	game();
	bool answer(const std::string&, const std::string&);
	void add_player(const std::string&, const std::string&);
	std::string get_next_problem();
	std::string get_problem();
	std::vector<player> get_players();
};