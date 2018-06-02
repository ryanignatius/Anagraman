#include "player.h"

player::player() {}

player::player(const std::string& id, const std::string& name) {
	this->id = id;
	this->display_name = name;
}

std::string player::get_id() const {
	return id;
}

std::string player::get_display_name() const {
	return (display_name.size() ? display_name : id);
}

int player::get_score() const {
	return score;
}

void player::add_score(const int& add) {
	score += add;
}