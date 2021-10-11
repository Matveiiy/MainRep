#include "Engine.h"
namespace sf {
	Evaluation* sf::Engine::Evaluate() { 
		return new Evaluation(__popcnt64(b.color[0].n) - __popcnt64(b.color[1].n)); 
	}
	Evaluation* sf::Engine::_go_max(const int& depth) {
		if (depth == 0) { return Evaluate(); }
		if (b.IsWin()) return new Evaluation(-WIN_SCORE);//CHECK!!!!!!!
		std::vector<uint32_t> moves; b.GenAllMoves(moves);
		Evaluation* current = new Evaluation(-INF_SCORE);
		Evaluation* temp;
		for (int i = 0; i < moves.size(); i++) {
			b.MakeSafeMove(moves[i]);
			if ((MOVE_TYPE(moves[i]) == sf::MoveType::JUMP || MOVE_TYPE(moves[i]) == sf::MoveType::PROMOTION_JUMP || MOVE_TYPE(moves[i]) == sf::MoveType::KING_JUMP) && b.CheckIfJumps()) {
				temp = _go_max(depth);
			}
			else {
				b.SwitchTurns();
				temp = _go_min(depth - 1);
				b.SwitchTurns();
			}
			if (temp->score > current->score) { 
				current->score = temp->score; 
				current->next = temp; 
				current->move = moves[i];
			}
			b.UndoMove();
		}
		return current;
	}
	Evaluation* sf::Engine::_go_min(const int& depth) {
		if (depth == 0) { return Evaluate(); }
		if (b.IsWin()) return new Evaluation(WIN_SCORE);//CHECK!!!!!!!
		std::vector<uint32_t> moves; b.GenAllMoves(moves);
		Evaluation* current = new Evaluation(INF_SCORE);
		Evaluation* temp;
		for (int i = 0; i < moves.size(); i++) {
			b.MakeSafeMove(moves[i]);
			if ((MOVE_TYPE(moves[i]) == sf::MoveType::JUMP || MOVE_TYPE(moves[i]) == sf::MoveType::PROMOTION_JUMP || MOVE_TYPE(moves[i]) == sf::MoveType::KING_JUMP)&& b.CheckIfJumps()) {
				temp = _go_min(depth);
			}
			else {
				b.SwitchTurns();
				temp = _go_max(depth - 1);
				b.SwitchTurns();
			}
			if (temp->score < current->score) {
				current->score = temp->score;
				current->next = temp;
				current->move = moves[i];
			}
			b.UndoMove();
		}
		return current;
	}
	
}