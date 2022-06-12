using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessEngine
{
    using Move = UInt32;
    using Score = Int32;
    enum Phase { Opening, Middlegame, Endgame }
    struct Eval
    {
        public Score score;
        public Move best;
        public Eval(Score score_, Move move)
        {
            this.score = score_;
            best = move;
        }
    }

    internal class Search
    {
        public Board board;
        public const Score SCORE_MATE = 32000;
        public const Score SCORE_DRAW = 0;
        public Search() { board = new Board(); }
        public Eval go(int depth)
        {
            if (Convert.ToBoolean(board.move_count & 1))
            {
                return goMax(depth);
            }
            else
            {
                return goMin(depth);
            }
        }
        public Eval goMin(int depth)
        {
            if (depth == 0) { return new Eval(StaticEval(), 0); }
            var Result = new Eval(SCORE_MATE, 0);
            var moves = board.GenBlackMoves();
            foreach (var move in moves)
            {
                //Board.print_move(move);
                board.MakeMove(move);
                var x = goMax(depth - 1);
                if (x.score < Result.score)
                {
                    Result.score = x.score;
                    Result.best = move;
                }
                board.UndoMove();
            }
            return Result;
        }
        public Eval goMax(int depth)
        {
            if (depth == 0) { return new Eval(StaticEval(), 0); }
            var Result = new Eval(-SCORE_MATE, 0);
            var moves = board.GenWhiteMoves();
            foreach (var move in moves)
            {
                //Board.print_move(move);
                board.MakeMove(move);
                var x = goMin(depth - 1);
                if (x.score > Result.score)
                {
                    Result.score = x.score;
                    Result.best = move;
                }
                board.UndoMove();
            }
            return Result;
        }
        public Score StaticEval()
        {
            if (Convert.ToBoolean(board.move_count & 1))
            {
                var moves = board.GenWhiteMoves();
                if (moves.Count == 0) return -SCORE_MATE;
                else return SCORE_DRAW;
            }
            else
            {
                var moves = board.GenBlackMoves();
                if (moves.Count == 0) return SCORE_MATE;
                else return SCORE_DRAW;
            }
        }
    }
}/*
    if (depth == 0)
        // return evaluation
        return evaluate();
    
    // increment nodes count
    nodes++;
    
    // best move so far
    int best_sofar;
    
    // old value of alpha
    int old_alpha = alpha;
    
    // create move list instance
    moves move_list[1];
    
    // generate moves
    generate_moves(move_list);
    
    // loop over moves within a movelist
    for (int count = 0; count < move_list->count; count++)
    {
        // preserve board state
        copy_board();
        
        // increment ply
        ply++;
        
        // make sure to make only legal moves
        if (make_move(move_list->moves[count], all_moves) == 0)
        {
            // decrement ply
            ply--;
            
            // skip to next move
            continue;
        }
        
        // score current move
        int score = -negamax(-beta, -alpha, depth - 1);
        
        // decrement ply
        ply--;

        // take move back
        take_back();
        
        // fail-hard beta cutoff
        if (score >= beta)
        {
            // node (move) fails high
            return beta;
        }
        
        // found a better move
        if (score > alpha)
        {
            // PV node (move)
            alpha = score;
            
            // if root move
            if (ply == 0)
                // associate best move with the best score
                best_sofar = move_list->moves[count];
        }
    }
    
    // found better move
    if (old_alpha != alpha)
        // init best move
        best_move = best_sofar;
    
    // node (move) fails low
    return alpha;
  */