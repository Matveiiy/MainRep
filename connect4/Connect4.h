#pragma once
#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#define BitBoardInt int64_t
#define INF 10000
#define MAX_SCORE 1000
#define MIN_SCORE -1000
#define PREFIX(xdfdfd) xdfdfd##  LL


namespace Connect4{
	class BitBoard {
		BitBoardInt data = 0;
		BitBoardInt BitCount();
	public:
		void Toggle(int& n) { data ^= 1ULL << n; }
		void Clear(int& n) { data &= ~(1ULL << n); }
		void Set(int& n) { data |= 1ULL << n; }
		void SetNum(int& n, int& number) { data ^= (-number ^ data) & (1ULL << n); }
		int Get(int& n) { return ((data >> n) & 1U); }
		BitBoard(BitBoardInt Data = 0) :data(Data) {}
		BitBoard operator|(const BitBoardInt& other) { return BitBoard(data | other); }
		BitBoard operator&(const BitBoardInt& other) { return BitBoard(data & other); }
		BitBoard operator^(const BitBoardInt& other) { return BitBoard(data ^ other); }
		BitBoard operator<<(const BitBoardInt& other) { return BitBoard(data << other); }
		BitBoard operator>>(const BitBoardInt& other) { return BitBoard(data >> other); }
		BitBoard operator~() { return BitBoard(~data); }

		BitBoard operator|(const BitBoard& other) { return data | other.data; }
		BitBoard operator&(const BitBoard& other) { return data & other.data; }
		BitBoard operator^(const BitBoard& other) { return data ^ other.data; }
		BitBoard operator<<(const BitBoard& other) { return data << other.data; }
		BitBoard operator>>(const BitBoard& other) { return data >> other.data; }

		void operator|=(const BitBoard& other) { data |= other.data; }
		void operator&=(const BitBoard& other) { data &= other.data; }
		void operator^=(const BitBoard& other) { data ^= other.data; }
		void operator<<=(const BitBoard& other) {data <<= other.data; }
		void operator>>=(const BitBoard& other) {data >>= other.data; }
		void operator=(BitBoardInt other) { data = other; }
		BitBoardInt get() { return data; }
	};
	class PositionEvaluation {
	public:
		int score;
		int8_t bestmove;
		PositionEvaluation(int Score=0, int8_t Bestmove=-1):score(Score), bestmove(Bestmove) {}
	};
	class Position{
	public:
		BitBoardInt bitboard[2];
		uint8_t counter=0;
		uint8_t height[7] = { 0, 7, 14, 21, 28, 35, 42 };
		std::vector<uint8_t> moves;
		Position() { moves = std::vector<uint8_t>(10000); }
		~Position() {}
		//----------------------
		//void Reset() {}
		bool IsYellowTurn() { !IsRedTurn(); }//divided by 2
		bool IsRedTurn() { counter & 1; }//not divided by 2
		void MakeMove(int col);
		void UndoMove();
		bool IsWin(BitBoardInt& bitboard);
		struct ListMove{
			int* arr;
			int siz;
			~ListMove() { delete[] arr; }
		} GenMoves();
	};
	class Gamev1 {
	public:
		Position position;
		Gamev1() {}
		void PlayerMove(int col) { position.MakeMove(col); }
		~Gamev1() {}
		PositionEvaluation go(int depth = 1);
	//private:
		PositionEvaluation evaluate();
		PositionEvaluation gomin(int depth = 1, int alpha=-INF, int beta=INF);
		PositionEvaluation gomax(int depth = 1, int alpha=-INF, int beta=INF);
	};
}