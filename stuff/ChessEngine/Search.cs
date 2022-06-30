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
    struct SearchResult
    {
        public Move best;
        public Score score;
        public int nodes;

    }
    internal class Search
    {
        public Board board;
        public const Score SCORE_MATE = 32000;
        public const Score SCORE_DRAW = 0;
        public const Score SCORE_INF = 32100;

        public Search() { board = new Board(); }

        public Move best_move = 0;
        public int ply = 0;
        public int nodes = 0;
        public SearchResult go(int depth)
        {
            SearchResult result = new SearchResult();
            ply = 0; nodes = 0;
            result.score = negamax(depth);
            result.best = best_move;
            result.nodes = nodes;
            return result;
        }
        public Score quiet_search(int alpha = -SCORE_INF, int beta = SCORE_INF)
        {
            nodes++;
            return StaticEval();
        }
        public Score negamax(int depth, int alpha = -SCORE_INF, int beta = SCORE_INF)
        {
            if (depth == 0) return StaticEval();
            nodes++;
            Move best_sofar=0;
            int old_alpha = alpha;
            List<Move> moves;
            if (Convert.ToBoolean(board.move_count & 1))
            {
                moves = board.GenWhiteMoves();
                if (moves.Count == 0)
                {
                    if (board.IsWhiteKingInCheck()) return -SCORE_MATE;
                    else return SCORE_DRAW;
                }
            }
            else
            {
                moves = board.GenBlackMoves();
                if (moves.Count == 0)
                {
                    if (board.IsBlackKingInCheck()) return -SCORE_MATE;
                    else return SCORE_DRAW;
                }
            }
            // loop over moves within a movelist
            foreach (var move in moves)
            {
                //Board.print_move(move);
                ply++;board.MakeMove(move);
                int score = -negamax(depth - 1, - beta, -alpha);
                ply--;board.UndoMove();
        
                
                if (score >= beta)
                {
                    // fail-hard beta cutoff(node fails high)
                    return beta;
                }
                if (score > alpha)
                {
                    //found better move; PV node (move)
                    alpha = score;
                    if (ply == 0)
                        best_sofar = move;
                }
            }
    
            // found better move
            if (old_alpha != alpha)
                // init best move
                best_move = best_sofar;
    
            // node (move) fails low
            return alpha;
        }
        public Score StaticEval()
        {
            var black_moves = board.GenBlackMoves();
            var white_moves = board.GenWhiteMoves();
            if (Convert.ToBoolean(board.move_count & 1))
            {
                if (white_moves.Count == 0)
                {
                    if (board.IsWhiteKingInCheck()) return -SCORE_MATE;
                    else return SCORE_DRAW;
                }
            }
            else
            {
                if (black_moves.Count == 0)
                {
                    if (board.IsBlackKingInCheck()) return -SCORE_MATE;
                    else return SCORE_DRAW;
                }
            }
            // Evaluate position ....
            return SCORE_DRAW;

        }
    }
}
/*

    // evaluate position
    int evaluation = evaluate();
    
    // fail-hard beta cutoff
    if (evaluation >= beta)
    {
        // node (move) fails high
        return beta;
    }
    
    // found a better move
    if (evaluation > alpha)
    {
        // PV node (move)
        alpha = evaluation;
    }
    
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
        if (make_move(move_list->moves[count], only_captures) == 0)
        {
            // decrement ply
            ply--;
            
            // skip to next move
            continue;
        }

        // score current move
        int score = -quiescence(-beta, -alpha);
        
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
            
        }
    }
    
    // node (move) fails low
    return alpha;
}
 */
/*
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