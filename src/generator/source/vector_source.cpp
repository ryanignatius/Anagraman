#include "vector_source.h"

vector_source::vector_source() {
	source = { "anjing", "kucing", "babi", "sapi", "kambing", "domba" };
}

vector_source::vector_source(std::string filename) {
	std::ifstream fin(filename);
	std::string line;
	while (std::getline(fin, line)) {
		source.emplace_back(line);
	}
}

size_t vector_source::size() {
	return source.size();
}

std::string vector_source::get_one(int index) {
	return source[index];
}

bool vector_source::is_existed(std::string test) {
	return std::find(source.begin(), source.end(), test) != source.end();
}