#include <algorithm>
#include "generator.h"

int main() {
	srand(time(NULL));
	generator mgenerator;
	problem mproblem = mgenerator.get_problem();
	std::cout << mproblem.get_ground() << std::endl;
	std::cout << mproblem.get_ground().size() << std::endl;
	for (int i = 0; i < 10; i++) {
		std::cout << mproblem.get_scrambled() << std::endl;
		mproblem.rescramble();
	}
}