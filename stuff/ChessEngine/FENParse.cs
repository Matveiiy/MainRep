                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
public class BoardStateData
{
     
    public string ActivePlayerColor = String.Empty;


    public string EnPassantSquare;
 
    public string[][] Ranks = new string[8][];

    public bool WhiteCanKingsideCastle;
    public bool WhiteCanQueensideCastle;
    public bool BlackCanKingsideCastle;
    public bool BlackCanQueensideCastle;
    public int HalfMoveCounter;
    public int FullMoveNumber;

     
    private void ParseRanks(string piecePlacementString)
    {
        string[] piecePlacementRanksArray = piecePlacementString.Split('/');

        for (int i = 0; i < piecePlacementRanksArray.Length; i++)
        {
            piecePlacementRanksArray[i] = SanitizeRank(piecePlacementRanksArray[i]);
        }

        for (int i = 0; i < piecePlacementRanksArray.Length; i++)
        {
            string[] allRanks = Array.ConvertAll(piecePlacementRanksArray[i].ToCharArray(), x => x.ToString());
            string[] newRank = new string[8];

            for (int j = 0; j < allRanks.Length; j++)
            {
                string thisSquare = allRanks[j];

                int n;
                if (int.TryParse(thisSquare, out n))
                {
                    int nullSquareCount = int.Parse(thisSquare);

                    for (int k = 0; k < nullSquareCount; k++)
                    {
                        newRank[j] = String.Empty;
                    }
                }
                else
                {
                    newRank[j] = thisSquare;
                }
            }

            Ranks[i] = newRank;
        }
    }

     
    private string SanitizeRank(string rank)
    {
        Regex r = new Regex(@"[\d]+");
        Match m = r.Match(rank);

        while (m.Success)
        {
            StringBuilder sb = new StringBuilder(rank);

            int index = m.Index;
            int nullSquareCount = int.Parse(m.Value);
            string newSubstring = String.Empty;

            for (int j = 0; j < nullSquareCount; j++)
            {
                newSubstring += " ";
            }

            sb.Remove(index, 1);
            sb.Insert(index, newSubstring);
            rank = sb.ToString();
            m = r.Match(rank);
        }

        return rank;
    }

    
    private void ParseActiveColor(string activeColorSubstring)
    {
        if (activeColorSubstring.ToLower() == "b")
        {
            ActivePlayerColor = "Black";
        }
        else if (activeColorSubstring.ToLower() == "w")
        {
            ActivePlayerColor = "White";
        }
    }

     
    private void ParseCastlingAvailability(string castlingAvailabilityString)
    {
        BlackCanKingsideCastle = false;
        BlackCanQueensideCastle = false;
        WhiteCanKingsideCastle = false;
        WhiteCanQueensideCastle = false;

        if (castlingAvailabilityString == "-")
        {
            return; 
        }
        else
        {
            if (castlingAvailabilityString.Contains("K"))
            {
                WhiteCanKingsideCastle = true;
            }

            if (castlingAvailabilityString.Contains("k"))
            {
                BlackCanKingsideCastle = true;
            }

            if (castlingAvailabilityString.Contains("Q"))
            {
                WhiteCanQueensideCastle = true;
            }

            if (castlingAvailabilityString.Contains("q"))
            {
                BlackCanQueensideCastle = true;
            }
        }
    }

    
    private void ParseEnPassantSquare(string enPassantSquareString)
    {
        if (enPassantSquareString == "-")
        {
            EnPassantSquare = String.Empty;
        }
        else if (!String.IsNullOrEmpty(enPassantSquareString))
        {
            EnPassantSquare = enPassantSquareString;
        }
    }

     
    private void ParseHalfMoveCounter(string halfmoveClockString)
    {
        HalfMoveCounter = int.Parse(halfmoveClockString);
    }

    
    private void ParseFullmoveNumber(string fullmoveNumberString)
    {
        FullMoveNumber = int.Parse(fullmoveNumberString);
    }

    
    public void ToConsole()
    {
        Console.WriteLine("-----------------");

        for (int i = 0; i < 8; i++)
        {
            string[] currentRank = Ranks[i];

            Console.WriteLine(
                  "|" + currentRank[0] + "|" + currentRank[1] + "|" + currentRank[2] + "|" + currentRank[3]
                + "|" + currentRank[4] + "|" + currentRank[5] + "|" + currentRank[6] + "|" + currentRank[7] + "|");
            Console.WriteLine("-----------------");
        }

        Console.WriteLine();
        Console.WriteLine("The active player is " + this.ActivePlayerColor + ".");

        if (this.WhiteCanKingsideCastle && this.WhiteCanQueensideCastle)
        {
            Console.Write("White can castle both king-side and queen-side, and ");
        }
        else if (this.WhiteCanKingsideCastle)
        {
            Console.Write("White can only castle on the king-side, and ");
        }
        else if (this.WhiteCanQueensideCastle)
        {
            Console.Write("White can only castle on the queen-side, and ");
        }
        else
        {
            Console.Write("White cannot castle, and ");
        }

        if (this.BlackCanKingsideCastle && this.BlackCanQueensideCastle)
        {
            Console.Write("Black can castle both king-side and queen-side.");
        }
        else if (this.BlackCanKingsideCastle)
        {
            Console.Write("Black can only castle on the king-side.");
        }
        else if (this.BlackCanQueensideCastle)
        {
            Console.Write("Black can only castle on the queen-side.");
        }
        else
        {
            Console.Write("Black cannot castle.");
        }

        Console.WriteLine();

        if (String.IsNullOrEmpty(this.EnPassantSquare))
        {
            Console.WriteLine("There are no squares eligible for en-passant capture.");
        }
        else
        {
            Console.WriteLine(this.EnPassantSquare + " is eligible for en-passant capture.");
        }

        if (this.HalfMoveCounter != null)
        {
            if (this.HalfMoveCounter == 1)
            {
                Console.WriteLine("There has been 1 halfmove since the last pawn advance or capture.");
            }
            else if (this.HalfMoveCounter == 0)
            {
                Console.WriteLine("This is the first halfmove since the last pawn advance or capture.");
            }
            else
            {
                Console.WriteLine("There have been " + this.HalfMoveCounter + " moves since the last pawn advance or capture.");
            }
        }

        if (this.FullMoveNumber != null)
        {
            if (this.FullMoveNumber == 1)
            {
                Console.WriteLine("This is the first move.");
            }
            else
            {
                Console.WriteLine("This is move number " + this.FullMoveNumber + ".");
            }
        }

        Console.ReadKey();
    }


    public BoardStateData() { }

    public BoardStateData(string fen)
    {
        string[] substrings = fen.Split(' ');
        ParseRanks(substrings[0]);
        ParseActiveColor(substrings[1]);
        ParseCastlingAvailability(substrings[2]);
        ParseEnPassantSquare(substrings[3]);
        ParseHalfMoveCounter(substrings[4]);
        ParseFullmoveNumber(substrings[5]);
    }

    //static void Main()
    //{
    //    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    //    BoardStateData boardStateData = new BoardStateData(fen);
    //    boardStateData.ToConsole();
    //}
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   