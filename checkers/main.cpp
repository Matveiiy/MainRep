#include <iostream>
#include <vector>
#include "Move.h"
#include "Engine.h"
using namespace std;
using namespace sf;

int main() {
	Board b1;
	b1.FromList({
		0, 2, 0, 2, 0, 2, 0, 2,
		2, 0, 2, 0, 2, 0, 2, 0,
		0, 2, 0, 2, 0, 2, 0, 2,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 0, 1, 0, 1, 0,
		0, 1, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 0, 1, 0, 1, 0
		});
	b1.SwitchTurns();
	/*b1.FromList({
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 2, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		3, 0, 3, 0, 1, 0, 1, 0
		});*/
	int k;
	std::vector<uint32_t> moves;
	Engine e(b1);
	Evaluation* ev= e._go_min(10);
	std::cout << ev->score << '\n';
	while (ev->next) {
		std::cout << sf::MoveToString(ev->move);
		ev=ev->next;
	}
	/*while (b1.color[0].n != 0 && b1.color[1].n != 0) {
		b1.GenAllMoves(moves);
		for (int i = 0; i < moves.size(); i++) std::cout << i << "----> "<< MoveToString(moves[i]);
		cin >> k;
		std::cout << '\n';
		b1.MakeSafeMove(moves[k]);
		if (MOVE_TYPE(moves[k]) == sf::JUMP || MOVE_TYPE(moves[k]) == sf::KING_JUMP || MOVE_TYPE(moves[k]) == sf::PROMOTION_JUMP) {}
		else b1.SwitchTurns();
		moves.clear();
	}*/
	return 0;
}
