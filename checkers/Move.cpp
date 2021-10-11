#include "Move.h"
sf::BitBoard all, empty;
sf::BitBoard b, j, f1, f2, f3, f4;
unsigned long lsb, last, temp;
unsigned long last1, last2, last3, last4;
unsigned int _if1 = 0;
std::vector<uint32_t> _hmoves;
namespace sf {
	std::string sf::MoveToString(uint32_t move) {
		std::string s;
		s += char('A' + MOVE_FROM(move) % 8);
		s += char('8' - MOVE_FROM(move) / 8);
		s += ' ';
		s += char('A' + MOVE_TO(move) % 8);
		s += char('8' - MOVE_TO(move) / 8);
		s += ' ';
		switch (MOVE_TYPE(move)) {
		case QUIET: s += "Quiet"; break;
		case JUMP: s += "Jump"; break;
		case PROMOTION: s += "Promotion"; break;
		case PROMOTION_JUMP: s += "Promotion_jump"; break;
		case KING_JUMP:  s += "King_jump"; break;
		case KING_MOVE:  s += "King_move"; break;
		}
		s += " (";
		s += char('A' + MOVE_TEMP(move) % 8);
		if (MOVE_TEMP(move) > 63) {
			s += char(char('8' - (MOVE_TEMP(move) - 64) / 8));
			s += ")--king\n";
		}
		else
		{
			s += char('8' - MOVE_TEMP(move) / 8);
			s += ")\n";
		}
		return s;
	}
	bool sf::Board::CheckIfJumps() {
		_hmoves.clear();
		GenKingJumps(_hmoves);
		if (turn) {
			GenPawnJumpDirL(9, notFileABB, _hmoves);
			GenPawnJumpDirL(7, notFileHBB, _hmoves);
		}
		else {
			GenPawnJumpDirR(9, notFileHBB, _hmoves);
			GenPawnJumpDirR(7, notFileABB, _hmoves);
		}
		return _hmoves.size();
	}
	inline void sf::Board::GenPawnJumpDirL(const int& dir, BitBoard const& file_bb, std::vector<uint32_t>& moves) {
		f1 = (color[turn] & (~k)).ShiftedL(dir) & file_bb & notRank8BB & color[!turn];
		j = f1.ShiftedL(dir) & file_bb & notRank8BB & empty;
		for (; j; j.ResetLSB()) {
			_BitScanForward64(&lsb, j.n);
			if (k.Check(lsb)) { _if1 = 64; }
			else _if1 = 0;
			if (lsb / 8 == 7) moves.push_back(ENCODE_MOVE(lsb - dir - dir, lsb, sf::PROMOTION_JUMP, _if1));
			else moves.push_back(ENCODE_MOVE(lsb - dir - dir, lsb, sf::JUMP, _if1));
		}
	}
	inline void sf::Board::GenPawnJumpDirR(const int& dir, BitBoard const& file_bb, std::vector<uint32_t>& moves) {
		f1 = (color[turn] & (~k)).ShiftedR(dir) & file_bb & notRank1BB & color[!turn];
		j = f1.ShiftedR(dir) & file_bb & notRank1BB & empty;
		for (; j; j.ResetLSB()) {
			_BitScanForward64(&lsb, j.n);
			if (k.Check(lsb)) { _if1 = 64; }
			else _if1 = 0;
			if (lsb / 8 == 0) moves.push_back(ENCODE_MOVE(lsb + dir + dir, lsb, sf::PROMOTION_JUMP, _if1));
			else moves.push_back(ENCODE_MOVE(lsb + dir + dir, lsb, sf::JUMP, _if1));
		}
	}
	inline void sf::Board::GenPawnMoveDirL(const int& dir, BitBoard const& file_bb, std::vector<uint32_t>& moves) {
		b = (color[turn] & (~k)).ShiftedL(dir) & file_bb & notRank8BB & empty;
		for (; b; b.n &= b.n - 1) {
			_BitScanForward64(&lsb, b.n);
			if (lsb / 8 == 7) moves.push_back(ENCODE_MOVE(lsb - dir, lsb, sf::PROMOTION, 0));
			else moves.push_back(ENCODE_MOVE(lsb - dir, lsb, sf::QUIET, 0));
		}
	}
	inline void sf::Board::GenPawnMoveDirR(const int& dir, BitBoard const& file_bb, std::vector<uint32_t>& moves) {
		b = (color[turn] & (~k)).ShiftedR(dir) & file_bb & notRank1BB & empty;
		for (; b; b.n &= b.n - 1) {
			_BitScanForward64(&lsb, b.n);
			if (lsb / 8 == 0) moves.push_back(ENCODE_MOVE(lsb + dir, lsb, sf::PROMOTION, 0));
			else moves.push_back(ENCODE_MOVE(lsb + dir, lsb, sf::QUIET, 0));
		}
	}
	
	inline void sf::Board::GenKingJumps(std::vector<uint32_t>& jumps) {
		b = color[turn] & k;
		for (_BitScanForward64(&lsb, b.n); b.n; b.ResetLSB(), _BitScanForward64(&lsb, b.n)) {
			f1 = raysRD[lsb] & all;
			f2 = raysRU[lsb] & all;
			f3 = raysLU[lsb] & all;
			f4 = raysLD[lsb] & all;
			_BitScanForward64(&last1, f1.n);
			_BitScanReverse64(&last2, f2.n);
			_BitScanReverse64(&last3, f3.n);
			_BitScanForward64(&last4, f4.n);
			//k.pretty();
			if (f1.n) {
				if (color[!turn].Check(last1)) {
					f1 = raysRD[last1] & all;
					_BitScanForward64(&temp, f1.n);
					if (temp == 0) temp = std::min(7 - int(last1) / 8, 7 - int(last1) % 8) * 9 + int(last1) + 1;
					if (k.Check((int)last1)) {
						for (int i = (int)last1 + 9; i < (int)temp; i += 9) {
							jumps.push_back(ENCODE_MOVE(lsb, i, KING_JUMP, last1 + 64));
						}
					}
					else {
						for (int i = (int)last1 + 9; i < (int)temp; i += 9) {
							jumps.push_back(ENCODE_MOVE(lsb, i, KING_JUMP, last1));
						}
					}
				}
			}
			if (f2.n) {
				if (color[!turn].Check(last2)) {
					f2 = raysRU[last2] & all;
					_BitScanForward64(&temp, f2.n);
					if (temp == 0) temp = std::min(int(last2) / 8, 7 - int(last2) % 8) * -7 + int(last2) - 1;
					if (k.Check((int)last2)) {
						for (int i = (int)last2 - 7; i > (int)temp; i -= 7) {
							jumps.push_back(ENCODE_MOVE(lsb, i, KING_JUMP, last2 + 64));
						}
					}
					else {
						for (int i = (int)last2 - 7; i > (int)temp; i -= 7) {
							jumps.push_back(ENCODE_MOVE(lsb, i, KING_JUMP, last2));
						}
					}
				}
			}
			if (f3.n) {
				if (color[!turn].Check(last3)) {
					f3 = raysLU[last3] & all;
					_BitScanForward64(&temp, f3.n);
					if (temp == 0) temp = std::min(int(last3) / 8, int(last3) % 8) * -9 + int(last3) - 1;
					if (k.Check((int)last3)) {
						for (int i = (int)last3 - 9; i > (int)temp; i -= 9) {
							jumps.push_back(ENCODE_MOVE(lsb, i, KING_JUMP, last3 + 64));
						}
					}
					else {
						for (int i = (int)last3 - 9; i > (int)temp; i -= 9) {
							jumps.push_back(ENCODE_MOVE(lsb, i, KING_JUMP, last3));
						}
					}
				}
			}
			if (f4.n) {
				if (color[!turn].Check(last4)) {
					f4 = raysLD[last4] & all;
					_BitScanForward64(&temp, f4.n);
					if (temp == 0) temp = std::min(7 - int(last4) / 8, int(last4) % 8) * 7 + int(last4) + 1;
					if (k.Check((int)last4)) {
						for (int i = (int)last4 + 7; i < (int)temp; i += 7) {
							jumps.push_back(ENCODE_MOVE(lsb, i, KING_JUMP, last4 + 64));
						}
					}
					else {
						for (int i = (int)last4 + 7; i < (int)temp; i += 7) {
							jumps.push_back(ENCODE_MOVE(lsb, i, KING_JUMP, last4));
						}
					}
				}
			}
		}
	}
	inline void sf::Board::GenKingMoves(std::vector<uint32_t>& moves) {
		b = color[turn] & k;
		for (_BitScanForward64(&lsb, b.n); b.n; b.ResetLSB(), _BitScanForward64(&lsb, b.n)) {
			f1 = raysRD[lsb] & all;
			f2 = raysRU[lsb] & all;
			f3 = raysLU[lsb] & all;
			f4 = raysLD[lsb] & all;
			_BitScanForward64(&last1, f1.n);
			_BitScanReverse64(&last2, f2.n);
			_BitScanReverse64(&last3, f3.n);
			_BitScanForward64(&last4, f4.n);
			if (last1 == 0) last1 = std::min(7 - lsb / 8, 7 - lsb % 8) * 9 + lsb + 1;
			if (last2 == 0) last2 = std::min(lsb / 8, 7 - lsb % 8) * -7 + lsb - 1;
			if (last3 == 0) last3 = std::min(lsb / 8, lsb % 8) * -9 + lsb - 1;
			if (last4 == 0) last4 = std::min(7 - lsb / 8, lsb % 8) * 7 + lsb + 1;
			for (int i = lsb + 9; i < (int)last1; i += 9) {
				moves.push_back(ENCODE_MOVE(lsb, i, KING_MOVE, 0));
			}
			for (long i = lsb - 7; i > (int)last2; i -= 7) {
				moves.push_back(ENCODE_MOVE(lsb, i, KING_MOVE, 0));
			}
			for (int i = lsb - 9; i > (int)last3; i -= 9) {
				moves.push_back(ENCODE_MOVE(lsb, i, KING_MOVE, 0));
			}
			for (int i = lsb + 7; i < (int)last4; i += 7) {
				moves.push_back(ENCODE_MOVE(lsb, i, KING_MOVE, 0));
			}
		}
	}
	void sf::Board::GenAllMovesSep(std::vector<uint32_t>& jumps, std::vector<uint32_t>& moves)
	{
		all = color[0] | color[1];
		empty = ~all;
		GenKingJumps(jumps);
		//
		if (turn) {
			//BLACK
			GenPawnJumpDirL(9, notFileABB, jumps);
			GenPawnJumpDirL(7, notFileHBB, jumps);
			if (jumps.size() == 0) {
				GenKingMoves(moves);
				GenPawnMoveDirL(9, notFileABB, moves);
				GenPawnMoveDirL(7, notFileHBB, moves);
			}
		}
		else {
			GenPawnJumpDirR(9, notFileHBB, jumps);
			GenPawnJumpDirR(7, notFileABB, jumps);
			if (jumps.size() == 0) {
				GenKingMoves(moves);
				GenPawnMoveDirR(7, notFileABB, moves);
				GenPawnMoveDirR(9, notFileHBB, moves);
			}
		}
	}
	void sf::Board::GenAllMoves(std::vector<uint32_t>& moves) { GenAllMovesSep(moves, moves); }
}