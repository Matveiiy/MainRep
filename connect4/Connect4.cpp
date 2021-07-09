#include "Connect4.h"

BitBoardInt move, siz, bb;
Connect4::PositionEvaluation pe;//
constexpr int directions[] = { 1, 7, 6, 8 };
constexpr int order[1] = {};
constexpr int minh(int a, int b) { return (a < b ? a : b); }
constexpr BitBoardInt TOP = 0b1000000100000010000001000000100000010000001000000L;

constexpr int table[] = {
	3, 4,   5,  5, 4, 3, 0,
	4, 6,   8,  8, 6, 4, 0,
	5, 8,  11, 11, 8, 5, 0,
	7, 10, 13, 13, 10, 7, 0,
	5, 8,  11, 11, 8, 5, 0,
	4, 6,   8,  8, 6, 4, 0,
	3, 4,   5,  5, 4, 3, 0,
};

//int getbit(int data, int n) { return ((data >> n) & 1U); }
int EvalTable(BitBoardInt b) {
	siz = 0;
	for (bb = 0; bb < 49; bb++, b >>= 1) { if (b & 1)siz += table[bb]; }
	return siz;
}
namespace Connect4 {
	void Position::MakeMove(int col)
	{
		move = PREFIX(1) << height[col]++; // (1)
		//std::cout << move;
		bitboard[counter & 1] ^= move;  // (2)
		moves[counter++] = col;         // (3)
	}
	void Position::UndoMove()
	{
		int col = moves[--counter];     // reverses (3)
		move = PREFIX(1) << --height[col]; // reverses (1)
		bitboard[counter & 1] ^= move;  // reverses (2)
	}
	bool Connect4::Position::IsWin(BitBoardInt& bitboard) {
		for (int direction : directions) {
			bb = bitboard & (bitboard >> direction);
			if ((bb & (bb >> (2 * direction))) != 0) return true;
		}
		return false;
	}
	Position::ListMove Position::GenMoves()
	{
		int* moves=new int[7];
		siz = 0;
		for (int col = 0; col <= 6; col++) {
			if ((TOP & (1LL << height[col])) == 0) moves[siz++] = col;
		}
		return { moves,(int)siz };
	}
	PositionEvaluation Gamev1::go(int depth)
	{
		if (position.counter & 1) return gomin(depth);
		else return gomax(depth);
	}
	PositionEvaluation Gamev1::evaluate()
	{
		if (position.IsWin(position.bitboard[0])) return PositionEvaluation(MAX_SCORE);
		if (position.IsWin(position.bitboard[1])) return PositionEvaluation(MIN_SCORE);
		siz = EvalTable(position.bitboard[0]) - EvalTable(position.bitboard[1]);
		return PositionEvaluation(siz);
	}
	PositionEvaluation Gamev1::gomin(int depth, int alpha, int beta)
	{
		if (depth == 0) return evaluate();
		if (position.IsWin(position.bitboard[1])) { return PositionEvaluation(MIN_SCORE-depth); }
		Position::ListMove m = position.GenMoves();
		PositionEvaluation eval(INF, -1);
		for (int i = 0; i < m.siz; i++) {
			position.MakeMove(m.arr[i]);
			pe = gomax(depth - 1, alpha, beta);//!!danger
			if (eval.score > pe.score) { eval.score = pe.score; eval.bestmove = m.arr[i]; }
			if (beta > pe.score) beta = pe.score;
			if (beta <= alpha) {
				position.UndoMove();
				break;
			}
			position.UndoMove();
		}
		return eval;
	}
	PositionEvaluation Gamev1::gomax(int depth, int alpha, int beta)
	{
		if (depth == 0) return evaluate();
		if (position.IsWin(position.bitboard[0])) { return PositionEvaluation(MAX_SCORE+depth); }
		Position::ListMove m = position.GenMoves();
		PositionEvaluation eval(-INF, -1);
		for (int i = 0; i < m.siz; i++) {
			position.MakeMove(m.arr[i]);
			pe = gomin(depth - 1, alpha, beta);//!!danger
			if (eval.score < pe.score) { eval.score = pe.score; eval.bestmove = m.arr[i]; }
			if (alpha < pe.score) alpha = pe.score;
			if (beta <= alpha) {
				position.UndoMove();
				break;
			}
			position.UndoMove();
		}
		return eval;
	}
}
