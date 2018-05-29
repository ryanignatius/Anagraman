#include <algorithm>
#include "generator.h"

int main() {
	srand(time(NULL));
	generator mgenerator;
	problem mproblem = mgenerator.get_problem();
	for (int i = 0; i < 10; i++) {
		std::cout << mproblem.get_scrambled() << std::endl;
		mproblem.rescramble();
	}
}