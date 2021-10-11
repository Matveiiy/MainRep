#pragma once
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>
#include "BitBoard.h"
#define ENCODE_MOVE(from_FFF, to_FFF, type_FF, temp_FF) (0UL | (from_FFF) | ((to_FFF) << 6) | ((type_FF) << 12) | ((temp_FF) << 16))
#define MOVE_FROM(move) ((move) & 0x3f)
#define MOVE_TO(move) (((move) >> 6) & 0x3f)
#define MOVE_TYPE(move) (((move) >> 12) & 0xf)
#define MOVE_TEMP(move) (((move) >> 16) & 0x7f)
namespace sf {
	//inline sf::Coord bitscanForward(const uint64_t& bb) {
	//	//    assert(bb != 0);
	//	return __builtin_ctzll(bb);
	//}

	enum Coord :uint8_t {
		A8, B8, C8, D8, E8, F8, G8, H8,
		A7, B7, C7, D7, E7, F7, G7, H7,
		A6, B6, C6, D6, E6, F6, G6, H6,
		A5, B5, C5, D5, E5, F5, G5, H5,
		A4, B4, C4, D4, E4, F4, G4, H4,
		A3, B3, C3, D3, E3, F3, G3, H3,
		A2, B2, C2, D2, E2, F2, G2, H2,
		A1, B1, C1, D1, E1, F1, G1, H1,
	};
	enum Piece :uint8_t {
		EMPTY = 0,
		PAWN = 1,
		KING = 2,
	};
	enum MoveType :uint8_t {
		QUIET = 0,
		JUMP = 1,
		PROMOTION = 2,
		PROMOTION_JUMP = 3,
		KING_MOVE = 4,
		KING_JUMP = 5,
	};
	std::string MoveToString(uint32_t move);
	struct Board {
	private:
		inline void _make_move(const uint8_t& from, const uint8_t& to, const uint8_t& mtype, uint8_t temp) {
			if (mtype == sf::MoveType::QUIET) {
				color[turn].Clear(from);
				color[turn].Set(to);
			}
			else if (mtype == sf::PROMOTION) {
				color[turn].Clear(from);
				color[turn].Set(to);
				k.Set(to);
			}
			else if (mtype == sf::JUMP) {
				color[turn].Clear(from);
				color[turn].Set(to);
				color[!turn].Clear((to + from) / 2);
				k.Clear((to + from) / 2);
			}
			else if (mtype == sf::PROMOTION_JUMP) {
				color[turn].Clear(from);
				color[turn].Set(to);
				k.Set(to);
				color[!turn].Clear((to + from) / 2);
				k.Clear((to + from) / 2);
			}
			else if (mtype == sf::MoveType::KING_MOVE) {
				k.Clear(from);
				color[turn].Clear(from);
				color[turn].Set(to);
				k.Set(to);
			}
			else {
				//else if (mtype == sf::MoveType::KING_JUMP) {
				k.Clear(from);
				color[turn].Clear(from);
				if (temp > 63) {
					temp -= 64;
					k.Clear(temp);
				}
				color[!turn].Clear(temp);
				color[turn].Set(to);
				k.Set(to);
			}
		}
		inline void GenPawnMoveDirR(const int& dir, BitBoard const& file_bb, std::vector<uint32_t>& moves);
		inline void GenPawnMoveDirL(const int& dir, BitBoard const& file_bb, std::vector<uint32_t>& moves);
		inline void GenPawnJumpDirL(const int& dir, BitBoard const& file_bb, std::vector<uint32_t>& moves);
		inline void GenPawnJumpDirR(const int& dir, BitBoard const& file_bb, std::vector<uint32_t>& moves);
	public:
		int counter = 0;
		bool turn = false;
		uint32_t history[1000] = {};
		BitBoard color[2] = { {}, {} };
		BitBoard k = {};
	public:
		inline void SwitchTurns() { turn = !turn; }
		Board() {

		}
		void PrettyPrint() {
			(color[0] | color[1]).pretty();
			std::cout << "------------------------------------------------------\n";
		}
		void FromList(std::initializer_list<int> list)  {
			if (list.size() != 64) throw "Bad array!";
			auto it = list.begin();
			k = 0;
			color[0] = 0;
			color[1] = 0;
			for (int c = 0; c < 64; c += 1, it++) {
				if ((c / 8 + c % 8) % 2 == 0) continue;
				if (*it == 0) continue;
				if (*it == 1) color[0].Set(c);
				else if (*it == 2) color[1].Set(c);
				else if (*it == 3) { color[0].Set(c); k.Set(c); }
				else if (*it == 4) { color[1].Set(c); k.Set(c); }
				else throw "Bad array!";
			}
		}
		void MakeSafeMove(const uint8_t& from, const uint8_t& to, const uint8_t& mtype, uint8_t temp = 0) { history[++counter] = ENCODE_MOVE(from, to, mtype, temp); _make_move(from, to, mtype, temp); }
		void MakeSafeMove(const uint32_t& move) { history[++counter] = move; _make_move(MOVE_FROM(move), MOVE_TO(move), MOVE_TYPE(move), MOVE_TEMP(move)); }
		inline void UndoMove() {
			uint8_t from = MOVE_FROM(history[counter]);
			uint8_t to = MOVE_TO(history[counter]);
			uint8_t mtype = MOVE_TYPE(history[counter]);
			uint8_t temp = MOVE_TEMP(history[counter]);
			if (mtype == sf::MoveType::QUIET) {
				color[turn].Clear(to);
				color[turn].Set(from);
			}
			else if (mtype == sf::PROMOTION) {
				color[turn].Clear(to);
				k.Clear(to);
				color[turn].Set(from);
			}
			else if (mtype == sf::MoveType::KING_MOVE) {
				k.Clear(to);
				color[turn].Clear(to);
				color[turn].Set(from);
				k.Set(from);
			}
			else if (mtype == sf::JUMP) {
				//TODO
				color[turn].Clear(to);
				color[turn].Set(from);
				color[!turn].Set((to + from) / 2);
				if (temp) k.Set((to + from) / 2);
			}
			else if (mtype == sf::PROMOTION_JUMP) {
				//TODO
				k.Clear(to);
				color[turn].Clear(to);
				color[turn].Set(from);
				color[!turn].Set((to + from) / 2);
				if (temp) k.Set((to + from) / 2);
			}
			else {
				//TODO
				//else if (mtype == sf::MoveType::KING_JUMP) {
				k.Clear(to);
				color[turn].Clear(to);
				color[turn].Set(from);
				k.Set(from);
				if (temp > 63) {
					temp -= 64;
					k.Set(temp);
				}
				color[!turn].Set(temp);
			}
			--counter;
		}
		inline void GenKingJumps(std::vector<uint32_t>& jumps);
		inline void GenKingMoves(std::vector<uint32_t>& moves);
		bool CheckIfJumps();
		inline bool IsWin() { return (color[0].n == 0 | color[1].n == 0); }
		void GenAllMoves(std::vector<uint32_t>& moves);
		void GenAllMovesSep(std::vector<uint32_t>& jumps, std::vector<uint32_t>& moves);
	};
}