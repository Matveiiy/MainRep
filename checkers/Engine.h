#pragma once
#include "Move.h"
#include <fstream>
#define INF_SCORE 10000
#define WIN_SCORE 1000
namespace sf {
	class Evaluation {
	public:
		int score;
		Evaluation* next;
		uint32_t move;
		Evaluation(int Score=0, Evaluation* Next=NULL):score(Score), next(Next) {}
		~Evaluation() { if (next) delete next; next = nullptr; }
	};
	class Engine {
		Board& b;
	public:
		Engine(Board& B) :b(B) { }
		~Engine() { }
		Evaluation* Evaluate();
		Evaluation* _go_min(const int& depth);
		Evaluation* _go_max(const int& depth);
		Evaluation* negamax(const int& depth);
		inline Evaluation* go(const int& depth) {
			return NULL;
			//return negamax(depth);
		}
	};
}