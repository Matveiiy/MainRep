import chess.pgn
pgn = open("simple.pgn")
file_resulting = open("simple_book.book", "w")
counter = -1
hash_map = {}
def count_iterable(i): return sum(1 for e in i)
while (True):
    game = chess.pgn.read_game(pgn)
    if game is None: break
    board = game.board()
    cur_move = game.variation(0)
    fen = board.fen()
    if fen in hash_map.keys(): pass
    else: hash_map[fen] = []
    board.push(cur_move.move)
    hash_map[fen].append(cur_move.move)
    counter+=1
    while (True):
        fen = board.fen()
        if fen in hash_map.keys(): pass
        else: hash_map[fen] = []
        cur_move = cur_move.next()
        if cur_move is None: break
        board.push(cur_move.move)
        hash_map[fen].append(cur_move.move)
    #if counter >= 5: break
    if counter%2500 == 0: print("Processed: ", counter)
existed = []
print("writting to file...")
for i in hash_map.keys():
    file_resulting.write(i+" => ")
    #file_resulting.write(i[:-4]+" => ")
    existed = []
    for j in hash_map[i]:
        k = str(j)
        if k in existed: continue
        file_resulting.write(k+" ")
        existed.append(k)
    file_resulting.write("\n")
print("Done!")
'''
import chess.pgn
pgn = open("simple.pgn")
file_resulting = open("simple_book.book", "w")
counter = -1
while (True):
    game = chess.pgn.read_game(pgn)
    if game is None: break
    counter+=1
    for i in game.mainline_moves():
        #print(i, end = ' ')
        file_resulting.write(str(i) + " ")
    file_resulting.write('\n')
    if counter % 10000 == 0: print("Processed: " + str(counter))
    #print('\n')
    #print(game)

'''

'''
from stockfish import Stockfish

params = {
    "Debug Log File": "",
    "Contempt": 0,
    "Min Split Depth": 0,
    "Threads": 1, # More threads will make the engine stronger, but should be kept at less than the number of logical processors on your computer.
    "Ponder": "false",
    "Hash": 16, # Default size is 16 MB. It's recommended that you increase this value, but keep it as some power of 2. E.g., if you're fine using 2 GB of RAM, set Hash to 2048 (11th power of 2).
    "MultiPV": 1,
    "Skill Level": 20,
    "Move Overhead": 10,
    "Minimum Thinking Time": 20,
    "Slow Mover": 100,
    "UCI_Chess960": "false",
    "UCI_LimitStrength": "false",
    "UCI_Elo": 1350
}
stockfish = Stockfish("stockfish.exe")
stockfish.set_fen_position("7r/8/8/2KP4/8/8/2k5/8 b - - 0 1")
stockfish.set_depth(15)
print(stockfish.get_best_move())
'''
'''
import subprocess
engineName = "stockfish"
p = subprocess.Popen(engineName, stdin=subprocess.PIPE,
                     stdout=subprocess.PIPE,
                     stderr=subprocess.STDOUT)
p.stdin.write(b"uci\n")
p.stdin.write(b"isready\n")
for eline in iter(p.stdout.readline, ''):
    print(eline)
    if eline.startswith(b"readyok"):
        break
'''