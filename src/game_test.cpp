#include <bits/stdc++.h>
#include "game/game.h"

using namespace std;

int main() {
	game mgame;
	for (int i = 0; i < 2; i++) {
		while (true) {
			cout << mgame.get_problem() << endl;
			string guess;
			getline(cin, guess);

			if (mgame.answer("dummy_user", guess)) {
				cout << "congrats!" << endl;
				mgame.get_next_problem();
				break;
			} else {
				cout << "uh oh! try again" << endl;
			}
		}
	}
}