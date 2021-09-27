#define VIRGO_IMPLEMENTATION
#include "virgo.h"
#include <iostream>
#include <string>
//--------------private----------------
virgo::Chessboard board;
int counter=1;
namespace sf{
    #define INF_SCORE 10000
    struct Evaluation{
        uint32_t move;
        int score;
        Evaluation(uint32_t m=0, float Score=0):move(m), score(Score) {}
    };
    void init() {
        virgo::virgo_init();
        board = virgo::position_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
    inline void setFEN(const std::string& s) {board=virgo::position_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");}
    inline void print() {std::cout << board << std::endl;}
    inline bool MakeWhiteMove(const uint32_t& m) {return virgo::make_move<WHITE>((uint32_t)m, board);}
    inline bool MakeBlackMove(const uint32_t& m) {return virgo::make_move<BLACK>((uint32_t)m, board);}
    inline void UndoWhiteMove() {virgo::take_move<WHITE>(board);}
    inline void UndoBlackMove() {virgo::take_move<BLACK>(board);}
    Evaluation go(int depth=1) {
        std::vector<uint16_t> legal_moves = {};
        if (counter%2==0) {virgo::get_legal_moves<WHITE>(board, legal_moves);}//WHITE MOVE
        else {virgo::get_legal_moves<BLACK>(board, legal_moves);} //black
        /*
        for(uint16_t & move : legal_moves) {
            std::cout << move << ' ' << MOVE_FROM(move) << ' ' << MOVE_TO(move) << ' ' << MOVE_TYPE(move) << '\n';
            std::cout << virgo::string::move_to_string(move) << std::endl;
        }
        */
        return Evaluation();
    }
}
int main() {
    sf::init();
    sf::go();
    return 0;
}
