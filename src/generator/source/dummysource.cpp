#include "dummysource.h"

dummysource::dummysource() {
	source = { "anjing", "kucing", "babi", "sapi", "kambing", "domba" };
}

size_t dummysource::size() {
	return source.size();
}

std::string dummysource::get_one(int index) {
	return source[index];
}

bool dummysource::is_existed(std::string test) {
	return std::find(source.begin(), source.end(), test) != source.end();
}