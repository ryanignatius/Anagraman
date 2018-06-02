#include "game.h"

void game::add_player(const std::string& user_id, const std::string& name = "") {
	if (players.find(user_id) == players.end()) players[user_id] = player(user_id, name);
}

void game::increment_score(const std::string& user_id, const int& score = 0) {
	int add = (score ? score : score_increment);
	players[user_id].add_score(add);
}

game::game() {
	current_problem = mgenerator.get_problem();
}

bool game::answer(const std::string& user_id, const std::string& guess) {
	if (!current_problem.check(guess)) {
		return false;
	}

	increment_score(user_id);
	return true;
}

std::string game::get_ground() {
	return current_problem.get_ground();
}

std::string game::get_next_problem() {
	current_problem = mgenerator.get_problem();
	return get_problem();
}

std::string game::get_problem() {
	return current_problem.get_scrambled();
}

std::vector<player> game::get_players() {
	std::vector<player> players;
	for (auto itplayer: this->players) {
		players.push_back(itplayer.second);
	}

	sort(players.begin(), players.end(), [&](const player& a, const player& b) {
		return a.get_score() > b.get_score();
	});

	return players;
}

std::string game::rescramble() {
	current_problem.rescramble();
	return current_problem.get_scrambled();
}
