using System.Runtime.CompilerServices;
namespace ChessEngine
{
    using bitboard = UInt64;
    using Move = UInt32;
    enum Piece { E, P, N, B, R, Q, K }
    enum ColorPiece { E, WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK }
    enum CastleEnum { E=0, WK, WQ, BK, BQ }
    enum MoveType
    {
        NORMAL = 0,
        PROMOTION = 1,
        EN_PASSANT = 2,
        CASTLES_WQ = 3,
        CASTLES_WK,
        CASTLES_BK,
        CASTLES_BQ
    }
    enum Square
    {
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8,
    }
    struct Board
    {
        const bitboard AllSquares = bitboard.MaxValue;
        const bitboard DarkSquares = 0xAA55AA55AA55AA55UL;
        const bitboard FileABB = 0x0101010101010101UL;
        const bitboard FileBBB = FileABB << 1;
        const bitboard FileCBB = FileABB << 2;
        const bitboard FileDBB = FileABB << 3;
        const bitboard FileEBB = FileABB << 4;
        const bitboard FileFBB = FileABB << 5;
        const bitboard FileGBB = FileABB << 6;
        const bitboard FileHBB = FileABB << 7;


        const bitboard NotAFile = ~FileABB;
        const bitboard NotBFile = ~FileBBB;
        const bitboard NotCFile = ~FileCBB;
        const bitboard NotDFile = ~FileDBB;
        const bitboard NotEFile = ~FileEBB;
        const bitboard NotFFile = ~FileFBB;
        const bitboard NotGFile = ~FileGBB;
        const bitboard NotHFile = ~FileHBB;

        const bitboard Not8Rank = ~Rank8BB;
        const bitboard Not1Rank = ~Rank1BB;

        const bitboard Rank1BB = 0xFF;
        const bitboard Rank2BB = Rank1BB << (8 * 1);
        const bitboard Rank3BB = Rank1BB << (8 * 2);
        const bitboard Rank4BB = Rank1BB << (8 * 3);
        const bitboard Rank5BB = Rank1BB << (8 * 4);
        const bitboard Rank6BB = Rank1BB << (8 * 5);
        const bitboard Rank7BB = Rank1BB << (8 * 6);
        const bitboard Rank8BB = Rank1BB << (8 * 7);


        private const bitboard One = 1UL;
        private const bitboard Ff = 0xFFUL;
        private const int MagicBishopDbLength = 512;
        private const int MagicRookDbLength = 4096;
        private static readonly bitboard[][] MagicBishopDb = new bitboard[64][];
        private static readonly bitboard[] MagicmovesBMagics =
            {
                0x0002020202020200UL, 0x0002020202020000UL, 0x0004010202000000UL, 0x0004040080000000UL, 0x0001104000000000UL, 0x0000821040000000UL, 0x0000410410400000UL, 0x0000104104104000UL,
                0x0000040404040400UL, 0x0000020202020200UL, 0x0000040102020000UL, 0x0000040400800000UL, 0x0000011040000000UL, 0x0000008210400000UL, 0x0000004104104000UL, 0x0000002082082000UL,
                0x0004000808080800UL, 0x0002000404040400UL, 0x0001000202020200UL, 0x0000800802004000UL, 0x0000800400A00000UL, 0x0000200100884000UL, 0x0000400082082000UL, 0x0000200041041000UL,
                0x0002080010101000UL, 0x0001040008080800UL, 0x0000208004010400UL, 0x0000404004010200UL, 0x0000840000802000UL, 0x0000404002011000UL, 0x0000808001041000UL, 0x0000404000820800UL,
                0x0001041000202000UL, 0x0000820800101000UL, 0x0000104400080800UL, 0x0000020080080080UL, 0x0000404040040100UL, 0x0000808100020100UL, 0x0001010100020800UL, 0x0000808080010400UL,
                0x0000820820004000UL, 0x0000410410002000UL, 0x0000082088001000UL, 0x0000002011000800UL, 0x0000080100400400UL, 0x0001010101000200UL, 0x0002020202000400UL, 0x0001010101000200UL,
                0x0000410410400000UL, 0x0000208208200000UL, 0x0000002084100000UL, 0x0000000020880000UL, 0x0000001002020000UL, 0x0000040408020000UL, 0x0004040404040000UL, 0x0002020202020000UL,
                0x0000104104104000UL, 0x0000002082082000UL, 0x0000000020841000UL, 0x0000000000208800UL, 0x0000000010020200UL, 0x0000000404080200UL, 0x0000040404040400UL, 0x0002020202020200UL
            };
        private static readonly bitboard[] MagicmovesBMask =
            {
                0x0040201008040200UL, 0x0000402010080400UL, 0x0000004020100A00UL, 0x0000000040221400UL, 0x0000000002442800UL, 0x0000000204085000UL, 0x0000020408102000UL, 0x0002040810204000UL,
                0x0020100804020000UL, 0x0040201008040000UL, 0x00004020100A0000UL, 0x0000004022140000UL, 0x0000000244280000UL, 0x0000020408500000UL, 0x0002040810200000UL, 0x0004081020400000UL,
                0x0010080402000200UL, 0x0020100804000400UL, 0x004020100A000A00UL, 0x0000402214001400UL, 0x0000024428002800UL, 0x0002040850005000UL, 0x0004081020002000UL, 0x0008102040004000UL,
                0x0008040200020400UL, 0x0010080400040800UL, 0x0020100A000A1000UL, 0x0040221400142200UL, 0x0002442800284400UL, 0x0004085000500800UL, 0x0008102000201000UL, 0x0010204000402000UL,
                0x0004020002040800UL, 0x0008040004081000UL, 0x00100A000A102000UL, 0x0022140014224000UL, 0x0044280028440200UL, 0x0008500050080400UL, 0x0010200020100800UL, 0x0020400040201000UL,
                0x0002000204081000UL, 0x0004000408102000UL, 0x000A000A10204000UL, 0x0014001422400000UL, 0x0028002844020000UL, 0x0050005008040200UL, 0x0020002010080400UL, 0x0040004020100800UL,
                0x0000020408102000UL, 0x0000040810204000UL, 0x00000A1020400000UL, 0x0000142240000000UL, 0x0000284402000000UL, 0x0000500804020000UL, 0x0000201008040200UL, 0x0000402010080400UL,
                0x0002040810204000UL, 0x0004081020400000UL, 0x000A102040000000UL, 0x0014224000000000UL, 0x0028440200000000UL, 0x0050080402000000UL, 0x0020100804020000UL, 0x0040201008040200UL
            };
        private static readonly bitboard[][] MagicRookDb = new bitboard[64][];
        private static readonly bitboard[] MagicmovesRMagics =
            {
                0x0080001020400080UL, 0x0040001000200040UL, 0x0080081000200080UL, 0x0080040800100080UL, 0x0080020400080080UL, 0x0080010200040080UL, 0x0080008001000200UL, 0x0080002040800100UL,
                0x0000800020400080UL, 0x0000400020005000UL, 0x0000801000200080UL, 0x0000800800100080UL, 0x0000800400080080UL, 0x0000800200040080UL, 0x0000800100020080UL, 0x0000800040800100UL,
                0x0000208000400080UL, 0x0000404000201000UL, 0x0000808010002000UL, 0x0000808008001000UL, 0x0000808004000800UL, 0x0000808002000400UL, 0x0000010100020004UL, 0x0000020000408104UL,
                0x0000208080004000UL, 0x0000200040005000UL, 0x0000100080200080UL, 0x0000080080100080UL, 0x0000040080080080UL, 0x0000020080040080UL, 0x0000010080800200UL, 0x0000800080004100UL,
                0x0000204000800080UL, 0x0000200040401000UL, 0x0000100080802000UL, 0x0000080080801000UL, 0x0000040080800800UL, 0x0000020080800400UL, 0x0000020001010004UL, 0x0000800040800100UL,
                0x0000204000808000UL, 0x0000200040008080UL, 0x0000100020008080UL, 0x0000080010008080UL, 0x0000040008008080UL, 0x0000020004008080UL, 0x0000010002008080UL, 0x0000004081020004UL,
                0x0000204000800080UL, 0x0000200040008080UL, 0x0000100020008080UL, 0x0000080010008080UL, 0x0000040008008080UL, 0x0000020004008080UL, 0x0000800100020080UL, 0x0000800041000080UL,
                0x0000102040800101UL, 0x0000102040008101UL, 0x0000081020004101UL, 0x0000040810002101UL, 0x0001000204080011UL, 0x0001000204000801UL, 0x0001000082000401UL, 0x0000002040810402UL
            };
        private static readonly bitboard[] MagicmovesRMask = {
                0x000101010101017EUL, 0x000202020202027CUL, 0x000404040404047AUL, 0x0008080808080876UL, 0x001010101010106EUL, 0x002020202020205EUL, 0x004040404040403EUL, 0x008080808080807EUL,
                0x0001010101017E00UL, 0x0002020202027C00UL, 0x0004040404047A00UL, 0x0008080808087600UL, 0x0010101010106E00UL, 0x0020202020205E00UL, 0x0040404040403E00UL, 0x0080808080807E00UL,
                0x00010101017E0100UL, 0x00020202027C0200UL, 0x00040404047A0400UL, 0x0008080808760800UL, 0x00101010106E1000UL, 0x00202020205E2000UL, 0x00404040403E4000UL, 0x00808080807E8000UL,
                0x000101017E010100UL, 0x000202027C020200UL, 0x000404047A040400UL, 0x0008080876080800UL, 0x001010106E101000UL, 0x002020205E202000UL, 0x004040403E404000UL, 0x008080807E808000UL,
                0x0001017E01010100UL, 0x0002027C02020200UL, 0x0004047A04040400UL, 0x0008087608080800UL, 0x0010106E10101000UL, 0x0020205E20202000UL, 0x0040403E40404000UL, 0x0080807E80808000UL,
                0x00017E0101010100UL, 0x00027C0202020200UL, 0x00047A0404040400UL, 0x0008760808080800UL, 0x00106E1010101000UL, 0x00205E2020202000UL, 0x00403E4040404000UL, 0x00807E8080808000UL,
                0x007E010101010100UL, 0x007C020202020200UL, 0x007A040404040400UL, 0x0076080808080800UL, 0x006E101010101000UL, 0x005E202020202000UL, 0x003E404040404000UL, 0x007E808080808000UL,
                0x7E01010101010100UL, 0x7C02020202020200UL, 0x7A04040404040400UL, 0x7608080808080800UL, 0x6E10101010101000UL, 0x5E20202020202000UL, 0x3E40404040404000UL, 0x7E80808080808000UL
            };
        private static readonly bitboard[] knight_table = new bitboard[64];
        private static readonly bitboard[,] pawn_attacks_table = new bitboard[2, 64];
        private static readonly bitboard[] king_attacks_table = new bitboard[64];


        public int move_count = 1;
        public bitboard cur_all =0, cur_black=0, cur_white=0, not_pinned = 0;
        public bitboard[] bboard;
        public ColorPiece[] piece_board;
        public Move[] moves = new Move[100];

        bool can_en_passant = false;
        bool[] can_castle;
        private void InitMagic() {
            for (var i = 0; i < MagicBishopDb.Length; i++)
                MagicBishopDb[i] = new bitboard[MagicBishopDbLength];

            for (var i = 0; i < MagicBishopDb.Length; ++i)
                MagicRookDb[i] = new bitboard[MagicRookDbLength];

            Span<int> initMagicMovesDb = stackalloc int[] { 63, 0, 58, 1, 59, 47, 53, 2,60, 39, 48,27,54,33,42,3,61,51,37,40,49,18,28,20,55,30, 34, 11,43,14,22,4,62,57,46,52,38,26,32,41,50,36,17,19,29,10,13,21,56,45,25,31,35,16,9,12,44,24,15,8, 23,7,6,5};

            Span<int> squares = stackalloc int[64];
            int numSquares;

            for (var i = 0; i < squares.Length; ++i)
            {
                numSquares = 0;
                var temp = MagicmovesBMask[i];
                while (temp != 0)
                {
                    var bit = (ulong)((long)temp & -(long)temp);
                    squares[numSquares++] = initMagicMovesDb[(int)((bit * 0x07EDD5E59A4E28C2UL) >> 58)];
                    temp ^= bit;
                }

                for (temp = 0; temp < One << numSquares; ++temp)
                {
                    var tempocc = InitmagicmovesOcc(squares.Slice(0, numSquares), temp);
                    MagicBishopDb[i][(tempocc * MagicmovesBMagics[i]) >> 55] = InitmagicmovesBmoves(i, tempocc);
                }
            }

            squares.Clear();

            for (var i = 0; i < squares.Length; ++i)
            {
                numSquares = 0;
                var temp = MagicmovesRMask[i];
                while (temp != 0)
                {
                    var bit = (ulong)((long)temp & -(long)temp);
                    squares[numSquares++] = initMagicMovesDb[(int)((bit * 0x07EDD5E59A4E28C2UL) >> 58)];
                    temp ^= bit;
                }

                for (temp = 0; temp < One << numSquares; ++temp)
                {
                    var tempocc = InitmagicmovesOcc(squares.Slice(0, numSquares), temp);
                    MagicRookDb[i][(tempocc * MagicmovesRMagics[i]) >> 52] = InitmagicmovesRmoves(i, tempocc);
                }
            }

            static ulong InitmagicmovesOcc(ReadOnlySpan<int> squares, ulong linocc)
            {
                var ret = 0ul;
                for (var i = 0; i < squares.Length; ++i)
                {
                    if ((linocc & (One << i)) != 0)
                        ret |= One << squares[i];
                }

                return ret;
            }
        }
        public Board() { can_castle = new bool[5]; piece_board = new ColorPiece[64]; bboard = new bitboard[13];moves[0] = 0; InitMagic(); for (var x = 0; x < 64; ++x) { knight_table[x] = GetKnightTablePart(x); king_attacks_table[x] = GetKingAttacksPart(x); pawn_attacks_table[0, x] = GetPawnAttacksTablePart(x, false); pawn_attacks_table[1, x] = GetPawnAttacksTablePart(x, true); } } //0 just empty
        bool is_valid_fen(string fen)
        {
            string[] substrings = fen.Split(' ');
            string side = substrings[1];
            string castleRights = substrings[2];
            string ep = substrings[3];
            if (side != "b" || side != "w")
            {
                return false;
            }
            if (castleRights != "-" && castleRights != "K" && castleRights != "Kk"
            && castleRights != "Kkq" && castleRights != "Kq" && castleRights != "KQ"
            && castleRights != "KQk" && castleRights != "KQq" && castleRights != "KQkq"
            && castleRights != "k" && castleRights != "q" && castleRights != "kq"
            && castleRights != "Q" && castleRights != "Qk" && castleRights != "Qq"
            && castleRights != "Qkq")
                return false;
            if (ep != "-")
            {
                if (ep.Length != 2) return false;
                if (!(ep[0] >= 'a' && ep[0] <= 'h')) return false;
                if (!((side == "w" && ep[1] == '6') || (side == "b" && ep[1] == '3')))
                    return false;
            }

            if (fen.Count(x => x == '/') != 7) return false;
            int wp = fen.Count(x => x == 'P');
            int bp = fen.Count(x => x == 'p');
            int wn = fen.Count(x => x == 'N');
            int bn = fen.Count(x => x == 'n');
            int wb = fen.Count(x => x == 'B');
            int bb = fen.Count(x => x == 'b');
            int wr = fen.Count(x => x == 'R');
            int br = fen.Count(x => x == 'r');
            int wq = fen.Count(x => x == 'Q');
            int bq = fen.Count(x => x == 'q');
            if (fen.Count(x => x == 'K') != 1) return false;
            if (fen.Count(x => x == 'k') != 1) return false;
            if (wp > 8 || bp > 8 || wn > 10 || bn > 10 || wb > 10 || bb > 10
            || wr > 10 || br > 10 || wq > 9 || bq > 10
            || wp + wn + wb + wr + wq > 15 || bp + bn + bb + br + bq > 15)
                return false;

            return true;
        }
        public bool parse_fen(string s)
        {
            var board_data = new BoardStateData(s);
            for (var x = 0; x < 13; ++x) { bboard[x] = 0; }
            for (var x = 0; x < 64; ++x) { piece_board[x] = ColorPiece.E; }

            can_castle[(int)CastleEnum.BK] = board_data.BlackCanKingsideCastle;
            can_castle[(int)CastleEnum.BQ] = board_data.BlackCanQueensideCastle;
            can_castle[(int)CastleEnum.WQ] = board_data.WhiteCanQueensideCastle;
            can_castle[(int)CastleEnum.WK] = board_data.WhiteCanKingsideCastle;
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                         if (board_data.Ranks[i][j] == "P") { piece_board[(7-i) * 8 + j] = ColorPiece.WP; Board.bitboard_set(ref bboard[(int)Piece.P], (7-i) * 8 + j); }
                    else if (board_data.Ranks[i][j] == "K") { piece_board[(7-i) * 8 + j] = ColorPiece.WK; Board.bitboard_set(ref bboard[(int)Piece.K], (7-i) * 8 + j); }
                    else if (board_data.Ranks[i][j] == "N") { piece_board[(7-i) * 8 + j] = ColorPiece.WN; Board.bitboard_set(ref bboard[(int)Piece.N], (7-i) * 8 + j); }
                    else if (board_data.Ranks[i][j] == "B") { piece_board[(7-i) * 8 + j] = ColorPiece.WB; Board.bitboard_set(ref bboard[(int)Piece.B], (7-i) * 8 + j); }
                    else if (board_data.Ranks[i][j] == "Q") { piece_board[(7-i) * 8 + j] = ColorPiece.WQ; Board.bitboard_set(ref bboard[(int)Piece.Q], (7-i) * 8 + j); }
                    else if (board_data.Ranks[i][j] == "R") { piece_board[(7-i) * 8 + j] = ColorPiece.WR; Board.bitboard_set(ref bboard[(int)Piece.R], (7-i) * 8 + j); }


                    else if (board_data.Ranks[i][j] == "p") { piece_board[(7-i) * 8 + j] = ColorPiece.BP; Board.bitboard_set(ref bboard[(int)Piece.P + 6], (7-i) * 8 + j); }
                    else if (board_data.Ranks[i][j] == "k") { piece_board[(7-i) * 8 + j] = ColorPiece.BK; Board.bitboard_set(ref bboard[(int)Piece.K + 6], (7-i) * 8 + j); }
                    else if (board_data.Ranks[i][j] == "n") { piece_board[(7-i) * 8 + j] = ColorPiece.BN; Board.bitboard_set(ref bboard[(int)Piece.N + 6], (7-i) * 8 + j); }
                    else if (board_data.Ranks[i][j] == "b") { piece_board[(7-i) * 8 + j] = ColorPiece.BB; Board.bitboard_set(ref bboard[(int)Piece.B + 6], (7-i) * 8 + j); }
                    else if (board_data.Ranks[i][j] == "q") { piece_board[(7-i) * 8 + j] = ColorPiece.BQ; Board.bitboard_set(ref bboard[(int)Piece.Q + 6], (7-i) * 8 + j); }
                    else if (board_data.Ranks[i][j] == "r") { piece_board[(7-i) * 8 + j] = ColorPiece.BR; Board.bitboard_set(ref bboard[(int)Piece.R + 6], (7-i) * 8 + j); }
                }
            } 

            return true;
        }
        public string get_fen()
        {
            int space = 0;
            string fen = "";
            for (int y = 7;y >= 0; --y)
            {
                for (int x = 0; x < 8; ++x)
                {
                    var cur_piece = piece_board[y * 8 + x];
                    if (cur_piece == ColorPiece.E)
                    {
                        space++;
                        //if (space == 8) { fen += space.ToString() + "/";space = 0; }
                    }
                    else
                    {
                        if (bitboard_getbit(bboard[(int)cur_piece], y*8+x))
                        {
                            //white
                            if (space == 0) fen += cur_piece.ToString();
                            else fen += space.ToString() + cur_piece.ToString();
                            space = 0;
                        }
                        else
                        {
                            if (space == 0) fen += cur_piece.ToString().ToLower();
                            else fen += space.ToString() + cur_piece.ToString().ToLower();
                            space = 0;
                        }
                    }
                }
                if (space != 0) { fen += space.ToString();space = 0; }
                fen += "/";
            }
            fen = fen.Remove(fen.Length - 1, 1);
            fen += " w - - 0 1";
            return fen;
        }
        public void InitMoveGeneration() { not_pinned = AllSquares; cur_white = bboard[1] | bboard[2] | bboard[3] | bboard[4] | bboard[5] | bboard[6];cur_black = bboard[7] | bboard[8] | bboard[9] | bboard[10] | bboard[11] | bboard[12];cur_all = cur_white | cur_black;}
        public List<Move> GenWhiteMoves()
        {
            not_pinned = AllSquares;
            cur_white = bboard[1] | bboard[2] | bboard[3] | bboard[4] | bboard[5] | bboard[6];
            cur_black = bboard[7] | bboard[8] | bboard[9] | bboard[10] | bboard[11] | bboard[12];
            cur_all = cur_white | cur_black;
            //white
            //todo
            List<Move> quiet = new List<Move>(), action = new List<Move>();
            
            GenWhitePawnMoves(ref quiet, ref action);
            GenQueenMoves(ref quiet, ref action, 0, ~cur_white, cur_black);
            GenRookMoves(ref quiet, ref action, 0, ~cur_white, cur_black);
            GenBishopMoves(ref quiet, ref action, 0, ~cur_white, cur_black);
            GenKnightMoves(ref quiet, ref action, 0, ~cur_white, cur_black);
            GenKingMoves(ref quiet, ref action, 0, ~cur_white, cur_black);
            
            List<Move> result=new List<Move>();
            foreach (var move in action) { MakeMove(move); InitMoveGeneration(); if (!IsWhiteKingInCheck()) result.Add(move); UndoMove();  }
            foreach (var move in quiet) { MakeMove(move); InitMoveGeneration(); if (!IsWhiteKingInCheck()) result.Add(move); UndoMove(); }
            GenWhiteCastlesKingside(ref result);
            GenWhiteCastlesQueenSide(ref result);
            return result;
        }
        public List<Move> GenBlackMoves() {
            not_pinned = AllSquares;
            cur_white = bboard[1] | bboard[2] | bboard[3] | bboard[4] | bboard[5] | bboard[6];
            cur_black = bboard[7] | bboard[8] | bboard[9] | bboard[10] | bboard[11] | bboard[12];
            cur_all = cur_white | cur_black;
            //white
            //todo
            List<Move> quiet = new List<Move>(), action = new List<Move>();
            GenBlackPawnMoves(ref quiet, ref action);
            GenQueenMoves(ref quiet, ref action, 6, ~cur_black, cur_white);
            GenRookMoves(ref quiet, ref action, 6, ~cur_black, cur_white);
            GenBishopMoves(ref quiet, ref action, 6, ~cur_black, cur_white);
            GenKnightMoves(ref quiet, ref action, 6, ~cur_black, cur_white);
            GenKingMoves(ref quiet, ref action, 6, ~cur_black, cur_white);

            List<Move> result = new List<Move>();
            foreach (var move in action) { MakeMove(move); InitMoveGeneration(); if (!IsBlackKingInCheck()) result.Add(move); UndoMove(); }
            foreach (var move in quiet) { MakeMove(move);  InitMoveGeneration(); if (!IsBlackKingInCheck()) result.Add(move); UndoMove(); }
            return result;
        }
        public void GenKnightMoves(ref List<Move> gmoves, ref List<Move> action, int offset, bitboard not_own, bitboard enemy)
        {
            bitboard available = bboard[(int)Piece.N + offset] & not_pinned;
            while (Convert.ToBoolean(available))
            {
                var pos = BitScanForward(available);
                var movesbb = KnightAttacks(pos);
                movesbb &= not_own;
                var actionbb = movesbb & enemy;
                movesbb = movesbb & (~enemy);
                while (Convert.ToBoolean(actionbb))
                {
                    var x = BitScanForward(actionbb);
                    action.Add(EncodeMove(MoveType.NORMAL, (Square)pos, (Square)x, (Byte)piece_board[x]));
                    ResetLSB(ref actionbb);
                }
                while (Convert.ToBoolean(movesbb))
                {
                    var x = BitScanForward(movesbb);
                    gmoves.Add(EncodeMove(MoveType.NORMAL, (Square)pos, (Square)x));
                    ResetLSB(ref movesbb);
                }
                ResetLSB(ref available);
            }
        }
        public void GenKingMoves(ref List<Move> gmoves, ref List<Move> action, int offset, bitboard not_own, bitboard enemy)
        {
            bitboard available = bboard[(int)Piece.K + offset] & not_pinned;
            while (Convert.ToBoolean(available))
            {
                var pos = BitScanForward(available);
                var movesbb = KingAttacks(pos);
                movesbb &= not_own;
                var actionbb = movesbb & enemy;
                movesbb = movesbb & (~enemy);
                while (Convert.ToBoolean(actionbb))
                {
                    var x = BitScanForward(actionbb);
                    action.Add(EncodeMove(MoveType.NORMAL, (Square)pos, (Square)x, (Byte)piece_board[x]));
                    ResetLSB(ref actionbb);
                }
                while (Convert.ToBoolean(movesbb))
                {
                    var x = BitScanForward(movesbb);
                    gmoves.Add(EncodeMove(MoveType.NORMAL, (Square)pos, (Square)x));
                    ResetLSB(ref movesbb);
                }
                ResetLSB(ref available);
            }
        }
        public void GenWhiteCastlesKingside(ref List<Move> gmoves)
        {
            if (!can_castle[(int)CastleEnum.WK]) return;
            if (IsWhiteKingInCheck()) return;
            var king = bboard[(int)ColorPiece.WK] << 1;
            if (IsWhiteKingInCheck() || Convert.ToBoolean(king & cur_all)) return;
            king <<= 1;
            if (IsWhiteKingInCheck() || Convert.ToBoolean(king & cur_all)) return;
            gmoves.Add(EncodeMove(MoveType.CASTLES_WK, Square.A1, Square.A1));
        }
        public void GenWhiteCastlesQueenSide(ref List<Move> gmoves)
        {
            if (!can_castle[(int)CastleEnum.WQ]) return;
            if (IsWhiteKingInCheck()) return;
            var king = bboard[(int)ColorPiece.WK] >> 1;
            if (IsWhiteKingInCheck() || Convert.ToBoolean(king & cur_all)) return;
            king >>= 1;
            if (IsWhiteKingInCheck() || Convert.ToBoolean(king & cur_all) || Convert.ToBoolean((king >> 1) & cur_all)) return;
            gmoves.Add(EncodeMove(MoveType.CASTLES_WQ, Square.A1, Square.A1));
        }
        public void GenWhitePawnMoves(ref List<Move> gmoves, ref List<Move> action)
        {
            bitboard available1 = not_pinned & bboard[(int)ColorPiece.WP];
            bitboard available2 = available1;
            bitboard quiet = (available2 << 8) & ~cur_all;
            bitboard doublemove = ((available1 & Rank2BB) << 8) & ~cur_all;
            doublemove = (doublemove << 8) & ~cur_all;

            available1 = (available1 << 9) & NotAFile;
            available2 = (available2 << 7) & NotHFile;
            available1 &= cur_black;
            available2 &= cur_black;
            var promoting1 = available1 & Rank8BB;
            var promoting2 = available2 & Rank8BB;
            var quiet_promote = quiet & Rank8BB;
            available1 &= Not8Rank;
            available2 &= Not8Rank;
            quiet &= Not8Rank;

            // !!!!!!!!!bug bacause moves and moves - the same name
            //Board.print_move(moves[move_count - 1]);
            //available = (available << 9);
            // 0 0 0   0 0 0   0 0 0
            // 0 1 1 & 0 1 0 = 0 1 0
            // 0 0 0   0 0 0   0 0 0 

            while (Convert.ToBoolean(promoting2))
            {
                var x = BitScanForward(promoting2);
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x - 7), (Square)x, (Byte)piece_board[x], (Byte)Piece.Q));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x - 7), (Square)x, (Byte)piece_board[x], (Byte)Piece.N));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x - 7), (Square)x, (Byte)piece_board[x], (Byte)Piece.R));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x - 7), (Square)x, (Byte)piece_board[x], (Byte)Piece.B));//todo: check
                ResetLSB(ref promoting2);
            }
            while (Convert.ToBoolean(promoting1))
            {
                var x = BitScanForward(promoting1);
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x - 9), (Square)x, (Byte)piece_board[x], (Byte)Piece.Q));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x - 9), (Square)x, (Byte)piece_board[x], (Byte)Piece.N));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x - 9), (Square)x, (Byte)piece_board[x], (Byte)Piece.R));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x - 9), (Square)x, (Byte)piece_board[x], (Byte)Piece.B));//todo: check
                ResetLSB(ref promoting1);
            }
            while (Convert.ToBoolean(quiet_promote))
            {
                var x = BitScanForward(quiet_promote);
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x - 8), (Square)x, 0, (Byte)Piece.Q));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x - 8), (Square)x, 0, (Byte)Piece.N));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x - 8), (Square)x, 0, (Byte)Piece.R));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x - 8), (Square)x, 0, (Byte)Piece.B));//todo: check
                ResetLSB(ref quiet_promote);
            }
            while (Convert.ToBoolean(available2))
            {
                var x = BitScanForward(available2);
                action.Add(EncodeMove(MoveType.NORMAL, (Square)(x - 7), (Square)x, (Byte)piece_board[x]));//todo: check
                ResetLSB(ref available2);
            }
            while (Convert.ToBoolean(available1))
            {
                var x = BitScanForward(available1);
                action.Add(EncodeMove(MoveType.NORMAL, (Square)(x - 9), (Square)x, (Byte)piece_board[x]));//todo: check
                ResetLSB(ref available1);
            }
            while (Convert.ToBoolean(doublemove))
            {
                var x = BitScanForward(doublemove);
                gmoves.Add(EncodeMove(MoveType.NORMAL, (Square)(x - 16), (Square)x));//todo: check
                ResetLSB(ref doublemove);
            }
            while (Convert.ToBoolean(quiet))
            {
                var x = BitScanForward(quiet);
                gmoves.Add(EncodeMove(MoveType.NORMAL, (Square)(x - 8), (Square)x));//todo: check
                ResetLSB(ref quiet);
            }
            //en_passant
            var en_passant_move = moves[move_count - 1];
            Board.print_move(en_passant_move);
            var to = (int)GetMoveTo(en_passant_move);

            if (piece_board[to] == ColorPiece.BP && ((int)GetMoveFrom(en_passant_move) / 8 == 6) && (to / 8 == 4)) {
                var en_try1 = to - 1;
                var en_try2 = to + 1;
                if (en_try2 / 8 == 4) if (bitboard_getbit(bboard[(int)Piece.P], en_try2)) { gmoves.Add(EncodeMove(MoveType.EN_PASSANT, (Square)en_try2, (Square)en_try2 + 7, (Byte)to, (Byte)piece_board[to])); }
                if (en_try1 / 8 == 4) if (bitboard_getbit(bboard[(int)Piece.P], en_try1)) { gmoves.Add(EncodeMove(MoveType.EN_PASSANT, (Square)en_try1, (Square)en_try1 + 7, (Byte)to, (Byte)piece_board[to])); }
            }
            
            //available = (available << 9);
            // 0 0 0   0 0 0   0 0 0
            // 0 1 1 & 0 1 0 = 0 1 0
            // 0 0 0   0 0 0   0 0 0 
        }
        public void GenBlackPawnMoves(ref List<Move> gmoves, ref List<Move> action)
        {
            bitboard available1 = not_pinned & bboard[(int)ColorPiece.BP];
            bitboard available2 = available1;
            bitboard quiet = (available2 >> 8) & ~cur_all;
            bitboard doublemove = ((available1 & Rank7BB) >> 8) & ~cur_all;
            doublemove = (doublemove >> 8) & ~cur_all;

            available1 = (available1 >> 9) & NotHFile;
            available2 = (available2 >> 7) & NotAFile;
            available1 &= cur_white;
            available2 &= cur_white;
            var promoting1 = available1 & Rank1BB;
            var promoting2 = available2 & Rank1BB;
            var quiet_promote = quiet & Rank1BB;
            available1 &= Not1Rank;
            available2 &= Not1Rank;
            quiet &= Not1Rank;


            //available = (available << 9);
            // 0 0 0   0 0 0   0 0 0
            // 0 1 1 & 0 1 0 = 0 1 0
            // 0 0 0   0 0 0   0 0 0 

            while (Convert.ToBoolean(promoting2))
            {
                var x = BitScanForward(promoting2);
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x + 7), (Square)x, (Byte)piece_board[x], (Byte)ColorPiece.BQ));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x + 7), (Square)x, (Byte)piece_board[x], (Byte)ColorPiece.BN));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x + 7), (Square)x, (Byte)piece_board[x], (Byte)ColorPiece.BR));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x + 7), (Square)x, (Byte)piece_board[x], (Byte)ColorPiece.BB));//todo: check
                ResetLSB(ref promoting2);
            }
            while (Convert.ToBoolean(promoting1))
            {
                var x = BitScanForward(promoting1);
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x + 9), (Square)x, (Byte)piece_board[x], (Byte)ColorPiece.BQ));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x + 9), (Square)x, (Byte)piece_board[x], (Byte)ColorPiece.BN));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x + 9), (Square)x, (Byte)piece_board[x], (Byte)ColorPiece.BR));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x + 9), (Square)x, (Byte)piece_board[x], (Byte)ColorPiece.BB));//todo: check
                ResetLSB(ref promoting1);
            }
            while (Convert.ToBoolean(quiet_promote))
            {
                var x = BitScanForward(quiet_promote);
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x + 8), (Square)x, 0, (Byte)ColorPiece.BQ));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x + 8), (Square)x, 0, (Byte)ColorPiece.BN));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x + 8), (Square)x, 0, (Byte)ColorPiece.BR));//todo: check
                action.Add(EncodeMove(MoveType.PROMOTION, (Square)(x + 8), (Square)x, 0, (Byte)ColorPiece.BB));//todo: check
                ResetLSB(ref quiet_promote);
            }
            while (Convert.ToBoolean(available2))
            {
                var x = BitScanForward(available2);
                action.Add(EncodeMove(MoveType.NORMAL, (Square)(x + 7), (Square)x, (Byte)piece_board[x]));//todo: check
                ResetLSB(ref available2);
            }
            while (Convert.ToBoolean(available1))
            {
                var x = BitScanForward(available1);
                action.Add(EncodeMove(MoveType.NORMAL, (Square)(x + 9), (Square)x, (Byte)piece_board[x]));//todo: check
                ResetLSB(ref available1);
            }
            while (Convert.ToBoolean(doublemove))
            {
                var x = BitScanForward(doublemove);
                gmoves.Add(EncodeMove(MoveType.NORMAL, (Square)(x + 16), (Square)x));//todo: check
                ResetLSB(ref doublemove);
            }
            while (Convert.ToBoolean(quiet))
            {
                var x = BitScanForward(quiet);
                gmoves.Add(EncodeMove(MoveType.NORMAL, (Square)(x + 8), (Square)x));//todo: check
                ResetLSB(ref quiet);
            }
            //available = (available << 9);
            // 0 0 0   0 0 0   0 0 0
            // 0 1 1 & 0 1 0 = 0 1 0
            // 0 0 0   0 0 0   0 0 0 
        }

        public void GenRookMoves(ref List<Move> gmoves, ref List<Move> action, int offset, bitboard not_own, bitboard enemy)
        {
            bitboard available = bboard[(int)Piece.R+offset] & not_pinned;
            int flag = 0;
            while (Convert.ToBoolean(available))
            {
                var pos = BitScanForward(available);
                var movesbb = RookAttacks(pos, cur_all);
                movesbb &= not_own;
                var actionbb = movesbb & enemy;
                movesbb = movesbb & (~actionbb);
                while (Convert.ToBoolean(actionbb))
                {
                    var x = BitScanForward(actionbb);
                    action.Add(EncodeMove(MoveType.NORMAL, (Square)pos, (Square)x, (Byte)piece_board[x], (Byte)flag));
                    ResetLSB(ref actionbb);
                }
                while (Convert.ToBoolean(movesbb))
                {
                    var x = BitScanForward(movesbb);
                    gmoves.Add(EncodeMove(MoveType.NORMAL, (Square)pos, (Square)x, 0, (Byte)flag));
                    ResetLSB(ref movesbb);
                }
                ResetLSB(ref available);
            }
        }
        public void GenBishopMoves(ref List<Move> gmoves, ref List<Move> action, int offset, bitboard not_own, bitboard enemy)
        {
            bitboard available = bboard[(int)Piece.B+offset] & not_pinned;
            while (Convert.ToBoolean(available))
            {
                var pos = BitScanForward(available);
                var movesbb = BishopAttacks(pos, cur_all);
                movesbb &= not_own;
                var actionbb = movesbb & enemy;
                movesbb = movesbb & (~actionbb);
                while (Convert.ToBoolean(actionbb))
                {
                    var x = BitScanForward(actionbb);
                    action.Add(EncodeMove(MoveType.NORMAL, (Square)pos, (Square)x, (Byte)piece_board[x]));
                    ResetLSB(ref actionbb);
                }
                while (Convert.ToBoolean(movesbb))
                {
                    var x = BitScanForward(movesbb);
                    gmoves.Add(EncodeMove(MoveType.NORMAL, (Square)pos, (Square)x));
                    ResetLSB(ref movesbb);
                }
                ResetLSB(ref available);
            }
        }
        public void GenQueenMoves(ref List<Move> gmoves, ref List<Move> action, int offset, bitboard not_own, bitboard enemy) { GenRookMoves(ref gmoves, ref action, offset + 1, not_own, enemy); GenBishopMoves(ref gmoves, ref action, offset + 2, not_own, enemy); }
        public int BitScanForward(bitboard x) { for (int i = 0; i < 64; ++i) { if (Board.bitboard_getbit(x, i)) return i; } return 0; }//very slow but fine for now
        public void ResetLSB(ref bitboard x) { x &= x - 1; }
        public bitboard GetWhitePawnMask() {var temp = bboard[(int)ColorPiece.WP];return ((temp << 9) & NotAFile) | ((temp << 7) & NotHFile);}
        //---------------------------------------------
        //------------||||||--MAYBE BUG-||||||--------
        //------------\/\/\/------------\/\/\/--------
        public bitboard GetBlackPawnMask() { var temp = bboard[(int)ColorPiece.BP]; return ((temp >> 9) & NotHFile) | ((temp >> 7) & NotAFile); }
        public bitboard GetAttackMask(int offset, bitboard pawn_mask) {
            //var bishops = bboard[(int)ColorPiece.BB];
            //var rooks = bboard[(int)ColorPiece.BR];
            //var knights = bboard[(int)ColorPiece.BN];
            //var queen = bboard[(int)ColorPiece.BQ];
            var attack_mask = pawn_mask;
            var temp = bboard[(int)Piece.B+offset];
            while (Convert.ToBoolean(temp))
            {
                attack_mask |= BishopAttacks(BitScanForward(temp), cur_all);
                ResetLSB(ref temp);
            }
            temp = bboard[(int)Piece.R + offset];
            while (Convert.ToBoolean(temp))
            {
                attack_mask |= RookAttacks(BitScanForward(temp), cur_all);
                ResetLSB(ref temp);
            }
            temp = bboard[(int)Piece.Q + offset];
            while (Convert.ToBoolean(temp))
            {
                attack_mask |= QueenAttacks(BitScanForward(temp), cur_all);
                ResetLSB(ref temp);
            }
            temp = bboard[(int)Piece.N + offset];
            while (Convert.ToBoolean(temp))
            {
                attack_mask |= KnightAttacks(BitScanForward(temp));
                ResetLSB(ref temp);
            }
            temp = bboard[(int)Piece.K + offset];
            while (Convert.ToBoolean(temp))
            {
                attack_mask |= KingAttacks(BitScanForward(temp));
                ResetLSB(ref temp);
            }
            //while ()
            return attack_mask;
        }
        public bool IsWhiteKingInCheck(bitboard black_attacks){ return Convert.ToBoolean(black_attacks & bboard[(int)ColorPiece.WK]); }public bool IsBlackKingInCheck(bitboard white_attacks) { return Convert.ToBoolean(white_attacks & bboard[(int)ColorPiece.BK]); }
        public bool IsWhiteKingInCheck() {return IsWhiteKingInCheck(GetAttackMask(6, GetBlackPawnMask()));}
        public bool IsBlackKingInCheck() { return IsBlackKingInCheck(GetAttackMask(0, GetWhitePawnMask())); }
        //public void GetWhiteKnightMoves(ref List<Move> moves) { throw new NotImplementedException(); }
        //public void GenWhiteBishopMoves(ref List<Move> moves) { throw new NotImplementedException(); }
        //public void GenWhiteRookMoves(ref List<Move> moves) { throw new NotImplementedException(); }
        //public void GenWhiteQueenMoves(ref List<Move> moves) { throw new NotImplementedException(); }
        //public void GenKingMoves(ref List<Move> moves) { throw new NotImplementedException(); }
        //public void GenPawnMoves(ref List<Move> moves) { throw new NotImplementedException(); }


        // 2 bits is movetype
        // 6 bit is square from
        // 6 bit is square to
        // 2+6+6 = 18 
        public void ShowBoard()
        {
            Console.WriteLine("-----------------");
            for (int i = 7; i >= 0; --i)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (Board.bitboard_getbit(bboard[1] | bboard[2] | bboard[3] | bboard[4] | bboard[5] | bboard[6], i * 8 + j))
                    {
                        //white
                        if (piece_board[i*8+j] == ColorPiece.WK) { Console.Write("|K"); }
                        else if (piece_board[i * 8 + j] == ColorPiece.WN) { Console.Write("|H"); }
                        else if (piece_board[i * 8 + j] == ColorPiece.WP) { Console.Write("|P"); }
                        else if (piece_board[i * 8 + j] == ColorPiece.WB) { Console.Write("|B"); }
                        else if (piece_board[i * 8 + j] == ColorPiece.WQ) { Console.Write("|Q"); }
                        else if (piece_board[i * 8 + j] == ColorPiece.WR) { Console.Write("|R"); }
                        //if (piece_board[i * 8 + j] == Piece.K) { Console.Write('♔'); }
                        //else if (piece_board[i * 8 + j] == Piece.Q) { Console.Write('♕'); }
                        //else if (piece_board[i * 8 + j] == Piece.R) { Console.Write('♖'); }
                        //else if (piece_board[i * 8 + j] == Piece.B) { Console.Write('♗'); }
                        //else if (piece_board[i * 8 + j] == Piece.N) { Console.Write('♘'); }
                        //else if (piece_board[i * 8 + j] == Piece.P) { Console.Write('♙'); }
                    }
                    else
                    {

                        if (piece_board[i * 8 + j] == ColorPiece.BK) { Console.Write("|k"); }
                        else if (piece_board[i * 8 + j] == ColorPiece.BN) { Console.Write("|h"); }
                        else if (piece_board[i * 8 + j] == ColorPiece.BP) { Console.Write("|p"); }
                        else if (piece_board[i * 8 + j] == ColorPiece.BB) { Console.Write("|b"); }
                        else if (piece_board[i * 8 + j] == ColorPiece.BQ) { Console.Write("|q"); }
                        else if (piece_board[i * 8 + j] == ColorPiece.BR) { Console.Write("|r"); }
                        //if (piece_board[i * 8 + j] == Piece.K) { Console.Write('♚'); }
                        //else if (piece_board[i * 8 + j] == Piece.Q) { Console.Write('♛'); }
                        //else if (piece_board[i * 8 + j] == Piece.R) { Console.Write('♜'); }
                        //else if (piece_board[i * 8 + j] == Piece.B) { Console.Write('♝'); }
                        //else if (piece_board[i * 8 + j] == Piece.N) { Console.Write('♞'); }
                        //else if (piece_board[i * 8 + j] == Piece.P) { Console.Write('♟'); }
                        //Console.Write(' ');
                        //else { if ((i + j) % 2 == 0) Console.Write(' ');else Console.Write('■'); }
                        else Console.Write("| ");
                    }
                    //Console.Write(' ');
                }
                Console.WriteLine("|");
                Console.WriteLine("-----------------");
            }
        }
        public void MakeMove(Move move) {
            int from = (int)GetMoveFrom(move);
            int to = (int)GetMoveTo(move);
            int captured = GetMoveParam1(move);
            switch (GetMoveType(move))
            {
                case MoveType.NORMAL:
                    Board.bitboard_set(ref bboard[(int)piece_board[from]], to);
                    Board.bitboard_clear(ref bboard[(int)piece_board[from]], from);
                    Board.bitboard_clear(ref bboard[captured], to);
                    piece_board[to] = piece_board[from];
                    piece_board[from] = ColorPiece.E;
                    can_castle[GetMoveParam2(move)] = false;
                    break;
                case MoveType.CASTLES_WQ:
                    Board.bitboard_clear(ref bboard[(int)ColorPiece.WK], (int)Square.E1);
                    Board.bitboard_clear(ref bboard[(int)ColorPiece.WR], (int)Square.A1);
                    Board.bitboard_set(ref bboard[(int)ColorPiece.WK], (int)Square.C1);
                    Board.bitboard_set(ref bboard[(int)ColorPiece.WR], (int)Square.D1);
                    piece_board[(int)Square.E1] = ColorPiece.E;
                    piece_board[(int)Square.D1] = ColorPiece.WR;
                    piece_board[(int)Square.C1] = ColorPiece.WK;
                    piece_board[(int)Square.A1] = ColorPiece.E;
                    can_castle[(int)CastleEnum.WQ] = false;
                    break;
                case MoveType.CASTLES_WK:
                    Board.bitboard_clear(ref bboard[(int)ColorPiece.WK], (int)Square.E1);
                    Board.bitboard_clear(ref bboard[(int)ColorPiece.WR], (int)Square.H1);
                    Board.bitboard_set(ref bboard[(int)ColorPiece.WK], (int)Square.G1);
                    Board.bitboard_set(ref bboard[(int)ColorPiece.WR], (int)Square.F1);
                    piece_board[(int)Square.E1] = ColorPiece.E;
                    piece_board[(int)Square.F1] = ColorPiece.WR;
                    piece_board[(int)Square.G1] = ColorPiece.WK;
                    piece_board[(int)Square.H1] = ColorPiece.E;
                    can_castle[(int)CastleEnum.WK] = false;
                    break;
                case MoveType.EN_PASSANT:
                    Board.bitboard_set(ref bboard[(int)piece_board[from]], to);
                    Board.bitboard_clear(ref bboard[(int)piece_board[from]], from);
                    Board.bitboard_clear(ref bboard[GetMoveParam2(move)], captured);
                    piece_board[to] = piece_board[from];
                    piece_board[from] = ColorPiece.E;
                    piece_board[captured] = ColorPiece.E;
                    break;
                case MoveType.PROMOTION:
                    int promoted = GetMoveParam2(move);
                    Board.bitboard_set(ref bboard[promoted], to);
                    Board.bitboard_clear(ref bboard[(int)piece_board[from]], from);
                    Board.bitboard_clear(ref bboard[captured], to);
                    piece_board[to] = (ColorPiece)promoted;
                    piece_board[from] = ColorPiece.E;
                    break;
                    
                default:
                    break;
            }
            moves[move_count++] = move;
            //Board.print_move(move);
        }
        public void UndoMove() {
            var move = moves[--move_count];
            int from = (int)GetMoveFrom(move);
            int to = (int)GetMoveTo(move);
            int captured = GetMoveParam1(move);
            switch (GetMoveType(move))
            {
                case MoveType.NORMAL:
                    Board.bitboard_set(ref bboard[(int)piece_board[to]], from);
                    Board.bitboard_clear(ref bboard[(int)piece_board[to]], to);
                    Board.bitboard_set(ref bboard[captured], to);
                    piece_board[from] = piece_board[to];
                    piece_board[to] = (ColorPiece)captured;
                    can_castle[GetMoveParam2(move)] = true;
                    break;
                case MoveType.EN_PASSANT:
                    var piece = GetMoveParam2(move);
                    Board.bitboard_set(ref bboard[(int)piece_board[to]], from);
                    Board.bitboard_clear(ref bboard[(int)piece_board[to]], to);
                    Board.bitboard_set(ref bboard[piece], captured);
                    piece_board[from] = piece_board[to];
                    piece_board[to] = ColorPiece.E;
                    piece_board[captured] = (ColorPiece)piece;
                    break;
                case MoveType.CASTLES_WQ:
                    Board.bitboard_set(ref bboard[(int)ColorPiece.WK], (int)Square.E1);
                    Board.bitboard_set(ref bboard[(int)ColorPiece.WR], (int)Square.A1);
                    Board.bitboard_clear(ref bboard[(int)ColorPiece.WK], (int)Square.C1);
                    Board.bitboard_clear(ref bboard[(int)ColorPiece.WR], (int)Square.D1);
                    piece_board[(int)Square.E1] = ColorPiece.WK;
                    piece_board[(int)Square.D1] = ColorPiece.E;
                    piece_board[(int)Square.C1] = ColorPiece.E;
                    piece_board[(int)Square.A1] = ColorPiece.WR;
                    can_castle[(int)CastleEnum.WQ] = true;
                    break;
                case MoveType.CASTLES_WK:
                    Board.bitboard_set(ref bboard[(int)ColorPiece.WK], (int)Square.E1);
                    Board.bitboard_set(ref bboard[(int)ColorPiece.WR], (int)Square.H1);
                    Board.bitboard_clear(ref bboard[(int)ColorPiece.WK], (int)Square.G1);
                    Board.bitboard_clear(ref bboard[(int)ColorPiece.WR], (int)Square.F1);
                    piece_board[(int)Square.E1] = ColorPiece.WK;
                    piece_board[(int)Square.F1] = ColorPiece.E;
                    piece_board[(int)Square.G1] = ColorPiece.E;
                    piece_board[(int)Square.H1] = ColorPiece.WR;
                    can_castle[(int)CastleEnum.WQ] = true;
                    break;
                case MoveType.PROMOTION:
                    var offset = (to > from) ? 0 : 6;
                    int promoted = GetMoveParam2(move);
                    Board.bitboard_clear(ref bboard[promoted], to);
                    Board.bitboard_set(ref bboard[(int)ColorPiece.WP+offset], from);
                    Board.bitboard_set(ref bboard[captured], to);
                    piece_board[to] = (ColorPiece)captured;
                    piece_board[from] = ColorPiece.WP + offset;
                    break;
            }

        }
        //[MethodImpl(MethodImplOptions.AggressiveInlining)] private void setbbpiece(ColorPiece piece, int num) { bitboard_set(ref bboard[(int)piece], num); }
        //[MethodImpl(MethodImplOptions.AggressiveInlining)] private void clearbbpiece(ColorPiece piece, int num) { bitboard_clear(ref bboard[(int)piece], num); }
        //public bitboard GetWhite() { return bboard[1] | bboard[2] | bboard[3] | bboard[4] | bboard[5] | bboard[6];  }
        //public bitboard GetBlack() { return bboard[7] | bboard[8] | bboard[9] | bboard[10] | bboard[11] | bboard[12]; }
        //public bitboard GetAll() { return GetWhite() | GetBlack(); }
        public static void shiftUR(ref bitboard bb) { bb <<= 9; } public static void shiftDL(ref bitboard bb) { bb >>= 9; }
        public static bitboard shiftedUR(bitboard bb) { return bb << 9; } public static bitboard shiftedDL(bitboard bb) { return bb >> 9; ; }
        public static void shiftUL(ref bitboard bb) { bb <<= 7; }public static void shiftDR(ref bitboard bb) { bb >>= 7; }
        public static bitboard shiftedUL(bitboard bb) { return bb << 7; }public static bitboard shiftedDR(bitboard bb) { return bb >> 7; }
        public static void bitboard_set(ref bitboard num, int n) { num |= 1UL << n; }
        public static void bitboard_clear(ref bitboard num, int n) { num &= ~(1UL << n); }
        public static bool bitboard_getbit(bitboard num, int n) { return Convert.ToBoolean((num >> n) & 1U); }
        public static void bitboard_print(bitboard num) { for (int i = 7; i >= 0; --i) { for (int j = 0; j < 8; ++j) { if (bitboard_getbit(num, i * 8 + j)) Console.Write("|1"); else Console.Write("|0"); } Console.WriteLine("|"); } Console.WriteLine("---------------"); }
        public static void print_move(Move move) {Console.Write(Board.GetMoveType(move));Console.Write(' ');Console.Write(Board.GetMoveFrom(move));Console.Write(' ');Console.Write(Board.GetMoveTo(move));Console.Write(' ');Console.Write((Piece)Board.GetMoveParam1(move));Console.Write(' ');Console.WriteLine((Piece)Board.GetMoveParam2(move));        }
        public static Move EncodeMove(MoveType type, Square from, Square to, Byte param1 = 0, Byte param2 = 0) { return ((UInt32)type << 28) | ((UInt32)from << 22) | ((UInt32)to << 16) | (UInt32)(param1 << 8) | (UInt32)(param2); }
        public static MoveType GetMoveType(Move move) { return (MoveType)(move >> 28); }//seems to work
        public static Square GetMoveFrom(Move move) { return (Square)((move >> 22) & 0b0000111111); } // seems to work
        public static Square GetMoveTo(Move move)   { return (Square)((move >> 16) & 0b0000000000111111); } // seems to work
        public static Byte GetMoveParam1(Move move)    { return (Byte)((move >> 8) & 0b000000000000000011111111); } // seems to work
        public static Byte GetMoveParam2(Move move)         { return (Byte)((move) & 0b00000000000000000000000011111111); } // seems to work
        public static bitboard BishopAttacks(int square, bitboard occupied) => MagicBishopDb[square][((occupied & MagicmovesBMask[square]) * MagicmovesBMagics[square]) >> 55];
        public static bitboard RookAttacks(int square, bitboard occupied) => MagicRookDb[square][((occupied & MagicmovesRMask[square]) * MagicmovesRMagics[square]) >> 52];
        public static bitboard QueenAttacks(int square, bitboard occupied) => BishopAttacks(square, occupied) | RookAttacks(square, occupied);
        public static bitboard KnightAttacks(int square) => knight_table[square];
        public static bitboard PawnAttacks(int square, int color) => pawn_attacks_table[color, square];
        public static bitboard KingAttacks(int square) => king_attacks_table[square];
        private static bitboard GetKnightTablePart(int x)
        {
            bitboard res = 0;
            int y = x / 8;
            x = x % 8;
            var offsety = new int[] { 2, -2, 1, -1,  2, -2, 1, -1};
            var offsetx = new int[] { 1, 1, 2, 2, -1, -1, -2, -2};
            for (var i = 0; i < 8; ++i)
            {
                var newx = offsetx[i] + x;
                var newy = offsety[i] + y;
                if (newx >=0 && newx <=7 && newy >=0 && newy <=7) { bitboard_set(ref res, newy * 8 + newx); }
            }
            return res;
        }
        private static bitboard GetPawnAttacksTablePart(int x, bool white)
        {

            bitboard res = 0;
            if (white)
            {
                x += 8;
                if (x % 8 != 7) bitboard_set(ref res, x + 1);
                if (x % 8 != 0) bitboard_set(ref res, x - 1);
                return res;
            }
            x -= 8;
            if (x % 8 != 7) bitboard_set(ref res, x + 1);
            if (x % 8 != 0) bitboard_set(ref res, x - 1);
            return res;
        }
        private static bitboard InitmagicmovesRmoves(int square, ulong occ)
        {
            var ret = 0ul;
            var rowbits = Ff << (8 * (square / 8));

            var bit = One << square;
            do
            {
                bit <<= 8;
                ret |= bit;
            }
            while (bit != 0 && (bit & occ) == 0);

            bit = One << square;
            do
            {
                bit >>= 8;
                ret |= bit;
            }
            while (bit != 0 && (bit & occ) == 0);

            bit = One << square;
            do
            {
                bit <<= 1;
                if ((bit & rowbits) != 0)
                    ret |= bit;
                else
                    break;
            }
            while ((bit & occ) == 0);

            bit = One << square;
            do
            {
                bit >>= 1;
                if ((bit & rowbits) != 0)
                    ret |= bit;
                else
                    break;
            }
            while ((bit & occ) == 0);

            return ret;
        }
        private static bitboard InitmagicmovesBmoves(int square, ulong occ)
        {
            var ret = 0UL;
            var rowbits = Ff << (8 * (square / 8));

            var bit = One << square;
            var bit2 = bit;
            do
            {
                bit <<= 8 - 1;
                bit2 >>= 1;
                if ((bit2 & rowbits) != 0)
                    ret |= bit;
                else
                    break;
            }
            while (bit != 0 && (bit & occ) == 0);

            bit = One << square;
            bit2 = bit;
            do
            {
                bit <<= 8 + 1;
                bit2 <<= 1;
                if ((bit2 & rowbits) != 0)
                    ret |= bit;
                else
                    break;
            }
            while (bit != 0 && (bit & occ) == 0);

            bit = One << square;
            bit2 = bit;
            do
            {
                bit >>= 8 - 1;
                bit2 <<= 1;
                if ((bit2 & rowbits) != 0)
                    ret |= bit;
                else
                    break;
            }
            while (bit != 0 && (bit & occ) == 0);

            bit = One << square;
            bit2 = bit;
            do
            {
                bit >>= 8 + 1;
                bit2 >>= 1;
                if ((bit2 & rowbits) != 0)
                    ret |= bit;
                else
                    break;
            }
            while (bit != 0 && (bit & occ) == 0);

            return ret;
        }
        private static bitboard GetKingAttacksPart(int square) {
            int x = square % 8;
            int y = square / 8;
            var arr = new int[] {
                x+1, y+1,
                x+1, y,
                x+1, y-1,
                x, y+1,
                x, y-1,
                x-1, y+1,
                x-1, y,
                x-1, y-1,
            };
            bitboard res = 0;
            for (var k = 0; k < arr.Length;k+=2)
            {
                var newx = arr[k];
                var newy = arr[k+1];
                if (newx >= 0 && newx <= 7 && newy >= 0 && newy <= 7) { bitboard_set(ref res, newy * 8 + newx); }
            }
            return res;
        }
    }
    class Entry
    {
        static void Main(string[] args)
        {
            //todo list:
            //  check pinned figures
            //  castles: 
            //     gen white/black pawn moves
            //     make/undo move
            // ---- less important ----
            //  rewrite fen parsing
            //  update bitscanforward(aka getLSB)
            //  add [MethodImpl(MethodImplOptions.AggressiveInlining)]
            //  change pawn move generation or not??
            // -------------------------------------------------------
            //  error: bishop disappears  capturing other bishop: rnbqkbnr/ppppppBp/8/8/P6P/1P4P1/2PPPP1R/RN1QKBN1 w - - 0 1
            //  en passant check: 8/3p4/8/4P3/8/8/8/8 w - - 0 1
            //  start position: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
            //  pawn test case: 4ppp1/3PP1pP/8/8/1p1pppP1/1pP1PP2/8/8 w - - 0 1
            //  castles test case: r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/R3K2R w KQkq - 4 4
            //bitboard b = 0;
            //Board.bitboard_set(ref b, (int)Square.A1);
            //Board.bitboard_print(b<<1);
            var engine = new Search();
            engine.board.parse_fen("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/R3K2R w KQkq - 4 4");
            var moves = engine.board.GenWhiteMoves();
            foreach (var move in moves)
            {
                Board.print_move(move);
                engine.board.MakeMove(move);
                engine.board.ShowBoard();
                engine.board.UndoMove();
            }
        }
    }
}