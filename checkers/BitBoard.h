#pragma once
#include <cstdint>
#include <iostream>
namespace sf {
	class BitBoard {
	public:
		uint64_t n;
		const BitBoard(const uint64_t& N=0):n(N) {}
		~BitBoard() {}
		explicit operator bool() const {return this->n;}
		inline BitBoard Seted(const int& k) const { return this->n | (1ULL << k); }
		inline void Set(const int& k) { this->n |= 1ULL << k; }
		inline void Toogle(const int& k) { this->n ^= 1ULL << k; }
		inline void Clear(const int& k) { this->n &= ~(1ULL << k); }
		inline BitBoard Cleared(const int& k) { return this->n & ~(1ULL << k); }
		inline void ResetLSB() { this->n &= this->n - 1; }
		inline void ResetMSB() {
			uint64_t mask = n;

			mask |= mask >> 1;
			mask |= mask >> 2;
			mask |= mask >> 4;
			mask |= mask >> 8;
			mask |= mask >> 16;

			mask = mask >> 1;

			n = n & mask;
		}
		inline bool Check(const int& k) const { return  (this->n >> k) & 1U; }//?
		inline uint64_t Get() const { return n; }
		inline BitBoard  operator~() const { return ~this->n; }
		//inline BitBoard friend operator~(const BitBoard&& b) { return ~b.n; }
		inline BitBoard  friend operator<<(const BitBoard& b, const BitBoard& s) { return b.n << s.n; }
		inline BitBoard  friend operator>>(const BitBoard& b, const BitBoard& s) { return b.n >> s.n; }
		inline BitBoard  friend operator&(const BitBoard&  b, const BitBoard& s) { return b.n & s.n; }
		inline BitBoard	 friend operator|(const BitBoard& b, const BitBoard& s) { return b.n | s.n; }
		inline BitBoard  friend operator^(const BitBoard& b, const BitBoard& s) { return b.n ^ s.n; }
		inline void operator|=(const BitBoard& b) { this->n |= b.n; }
		inline void operator^=(const BitBoard& b) { this->n ^= b.n; }
		inline void operator&=(const BitBoard& b) { this->n &= b.n; }
		inline void operator<<=(const BitBoard& b) { this->n <<= b.n; }
		inline void operator>>=(const BitBoard& b) { this->n >>= b.n; }
		inline BitBoard ShiftedL(const int& data) { return this->n << data; }
		inline BitBoard ShiftedR(const int& data) { return this->n >> data; }
		inline BitBoard ShiftedR1() { return this->n << 1; }
		inline BitBoard ShiftedL1() { return this->n >> 1; }
		inline BitBoard ShiftedU1() { return this->n >> 8; }
		inline BitBoard ShiftedD1() { return this->n << 8; }
		inline BitBoard ShiftedRU() { return this->n >> 7; }
		inline BitBoard ShiftedLD() { return this->n << 7; }
		inline BitBoard ShiftedRD() { return this->n << 9; }
		inline BitBoard ShiftedLU() { return this->n >> 9; }
		inline void Shift(const int& data) { this->n <<= data; }
		inline void ShiftR() { this->n <<= 1; }
		inline void ShiftL() { this->n >>= 1; }
		inline void ShiftU() { this->n >>= 8; }
		inline void ShiftD() { this->n <<= 8; }
		inline void ShiftRU() { this->n >>= 7; }
		inline void ShiftLD() { this->n <<= 7; }
		inline void ShiftRD() { this->n <<= 9; }
		inline void ShiftLU() { this->n >>= 9; } 
		void pretty() const {
			for (int i = 0; i < 64; i += 8) {
				for (int j = i; j < i + 8; j++) {
					std::cout << Check(j) << ' ';
				}
				std::cout << '\n';
			}
		}
	};

	static const BitBoard FileABB = 0x0101010101010101ULL;
	static const BitBoard FileHBB = FileABB << 7;
	static const BitBoard notFileABB = ~FileABB;
	static const BitBoard notFileHBB = ~FileHBB;

	static const BitBoard Rank8BB = 0xFF;
	static const BitBoard Rank1BB = Rank8BB << (8 * 7);

	static const BitBoard notRank1BB = ~Rank1BB;
	static const BitBoard notRank8BB = ~Rank8BB;

	static BitBoard raysRU[64] = {
		0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 8, 16, 32, 64, 128, 0, 516, 1032, 2064, 4128, 8256, 16512, 32768, 0, 132104, 264208, 528416, 1056832, 2113664, 4227072, 8388608, 0, 33818640, 67637280, 135274560, 270549120, 541097984, 1082130432, 2147483648, 0, 8657571872, 17315143744, 34630287488, 69260574720, 138521083904, 277025390592, 549755813888, 0, 2216338399296, 4432676798592, 8865353596928, 17730707128320, 35461397479424, 70918499991552, 140737488355328, 0, 567382630219904, 1134765260439552, 2269530520813568, 4539061024849920, 9078117754732544, 18155135997837312, 36028797018963968, 0
	};
	static BitBoard raysLD[64] = {
		0, 256, 66048, 16909312, 4328785920, 1108169199616, 283691315109888, 72624976668147712, 0, 65536, 16908288, 4328783872, 1108169195520, 283691315101696, 72624976668131328, 145249953336262656, 0, 16777216, 4328521728, 1108168671232, 283691314053120, 72624976666034176, 145249953332068352, 290499906664136704, 0, 4294967296, 1108101562368, 283691179835392, 72624976397598720, 145249952795197440, 290499905590394880, 580999811180789760, 0, 1099511627776, 283673999966208, 72624942037860352, 145249884075720704, 290499768151441408, 580999536302882816, 1161999072605765632, 0, 281474976710656, 72620543991349248, 145241087982698496, 290482175965396992, 580964351930793984, 1161928703861587968, 2323857407723175936, 0, 72057594037927936, 144115188075855872, 288230376151711744, 576460752303423488, 1152921504606846976, 2305843009213693952, 4611686018427387904, 0, 0, 0, 0, 0, 0, 0, 0
	};
	static BitBoard raysRD[64] = {
		9241421688590303744, 36099303471055872, 141012904183808, 550831656960, 2151686144, 8404992, 32768, 0, 4620710844295151616, 9241421688590303232, 36099303471054848, 141012904181760, 550831652864, 2151677952, 8388608, 0, 2310355422147510272, 4620710844295020544, 9241421688590041088, 36099303470530560, 141012903133184, 550829555712, 2147483648, 0, 1155177711056977920, 2310355422113955840, 4620710844227911680, 9241421688455823360, 36099303202095104, 141012366262272, 549755813888, 0, 577588851233521664, 1155177702467043328, 2310355404934086656, 4620710809868173312, 9241421619736346624, 36099165763141632, 140737488355328, 0, 288793326105133056, 577586652210266112, 1155173304420532224, 2310346608841064448, 4620693217682128896, 9241386435364257792, 36028797018963968, 0, 144115188075855872, 288230376151711744, 576460752303423488, 1152921504606846976, 2305843009213693952, 4611686018427387904, 9223372036854775808, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	static BitBoard raysLU[64] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 8, 16, 32, 64, 0, 256, 513, 1026, 2052, 4104, 8208, 16416, 0, 65536, 131328, 262657, 525314, 1050628, 2101256, 4202512, 0, 16777216, 33619968, 67240192, 134480385, 268960770, 537921540, 1075843080, 0, 4294967296, 8606711808, 17213489152, 34426978560, 68853957121, 137707914242, 275415828484, 0, 1099511627776, 2203318222848, 4406653222912, 8813306511360, 17626613022976, 35253226045953, 70506452091906, 0, 281474976710656, 564049465049088, 1128103225065472, 2256206466908160, 4512412933881856, 9024825867763968, 18049651735527937
	};
}