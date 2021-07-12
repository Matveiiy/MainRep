#include <iostream>
#include <queue>
#include <list>
#define WALK_RD 9
#define WALK_LD 7
#define WALK_RU -7
#define WALK_LU -9
#define JUMP_RD 18
#define JUMP_LD 14
#define JUMP_RU -14
#define JUMP_LU -18
//RIGHT=>notAFile
//LEFT=>notHFile
using namespace std;
#define BitBoard uint64_t
const BitBoard notAFile = 0xfefefefefefefefe; // ~0x0101010101010101
const BitBoard notHFile = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080
BitBoard MoveNW(BitBoard k) {
    return (k << 9) & notAFile;
}
class State {
public:
    BitBoard b, w, k;
    BitBoard OrAll() { return w | b; }
    State(BitBoard W = 0, BitBoard B = 0, BitBoard K = 0) :b(B), w(W), k(K) {}
    friend bool operator==(State& a, State& b) {
        return (a.w == b.w) && (a.b == b.b) && (a.k == b.k);
    }
};
// positve left, negative right shifts
const int shift[8] = { 9, 1,-7,-8,-9,-1, 7, 8 };
const int walks[4] = { 9, 7, -7, -9 };
const int jumps[4] = { 18, 14, -14, -18 };
class Board {

public:
    State m;
    State moves[1000];
    int counter = 0;
    void FromArray(int* arr) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (arr[i * 8 + j] == 1) { Set(m.w, i * 8 + j); }
                else if (arr[i * 8 + j] == 2) { Set(m.b, i * 8 + j); }
                else if (arr[i * 8 + j] == 3) { Set(m.w, i * 8 + j); Set(m.k, i * 8 + j); }
                else if (arr[i * 8 + j] == 4) { Set(m.b, i * 8 + j); Set(m.k, i * 8 + j); }
            }
        } 
        moves[0] = m;
    }
    static void Set(BitBoard& number, int n) { number |= 1ULL << n; }
    static bool Get(BitBoard number, int n) { return (number >> n) & 1U; }
    void MakeMove(State M) { m = M; moves[++counter] = M; }
    void UndoMove() { m = moves[--counter]; }
    list<State> GenJumpsWhite() {
        list<State> listm;
        queue<State> q;
        BitBoard tl, td, tk;
        BitBoard block, wp, cap, fin, tempi, tempj, k = 0;
        State nextstate;
        q.push(m);
        while (!q.empty()) {
            this->MakeMove(q.front());
            q.pop();
            block = m.b | m.w;
            wp = m.w & ~m.k;
            k = 0;
            for (int i = 0; i < 64; i++) {
                if (Get(wp, i)) {
                    tempj = i % 8;
                    tempi = i / 8;
                    if (tempi < 6 && tempj < 6) {
                        //--DOWN RIGHT--
                        cap = 1ULL << (i + WALK_RD);
                        fin = 1ULL << (i + JUMP_RD);
                        if ((cap & m.b) && !(fin & block)) {
                            //MOVE
                            k++;
                            tl = m.w & ~(1ULL << i);
                            td = m.b & ~cap;
                            tk = m.k & ~cap;
                            tl = tl | fin;
                            q.push(State(tl, td, tk));
                        }
                    }
                    if (tempi > 1 && tempj < 6) {
                        //--UP RIGHT--
                        cap = 1ULL << (i + WALK_RU);
                        fin = 1ULL << (i + JUMP_RU);
                        if ((cap & m.b) && !(fin & block)) {
                            //MOVE
                            k++;
                            tl = m.w & ~(1ULL << i);
                            td = m.b & ~cap;
                            tk = m.k & ~cap;
                            tl = tl | fin;
                            q.push(State(tl, td, tk));
                        }
                    }
                    if (tempi > 1 && tempj > 1) {
                        //--UP LEFT--
                        cap = 1ULL << (i + WALK_LU);
                        fin = 1ULL << (i + JUMP_LU);
                        if ((cap & m.b) && !(fin & block)) {
                            //MOVE
                            k++;
                            tl = m.w & ~(1ULL << i);
                            td = m.b & ~cap;
                            tk = m.k & ~cap;
                            tl = tl | fin;
                            q.push(State(tl, td, tk));
                        }
                    }
                    if (tempi < 6 && tempj>1) {
                        //DOWN LEFT
                        cap = 1ULL << (i + WALK_LD);
                        fin = 1ULL << (i + JUMP_LD);
                        if ((cap & m.b) && !(fin & block)) {
                            k++;
                            tl = m.w & ~(1ULL << i);
                            td = m.b & ~cap;
                            tk = m.k & ~cap;
                            tl = tl | fin;
                            q.push(State(tl, td, tk));
                            this->MakeMove(q.front());
                            this->UndoMove();
                        }
                    }
                    /*if i*/
                }
            }
            if (k == 0) { listm.push_back(this->moves[this->counter]); }
            this->UndoMove();
        }
        return listm;
    }
    list<State> GenJumpsBlack() {
        list<State> listm;
        queue<State> q;
        BitBoard tl, td, tk;
        BitBoard block, wp, cap, fin, tempi, tempj, k = 0;
        State nextstate;
        q.push(m);
        while (!q.empty()) {
            this->MakeMove(q.front());
            q.pop();

            block = m.b | m.w;
            wp = m.b & ~m.k;//1
            k = 0;
            for (int i = 0; i < 64; i++) {
                if (Get(wp, i)) {
                    tempj = i % 8;
                    tempi = i / 8;
                    if (tempi < 6 && tempj < 6) {
                        //--DOWN RIGHT--
                        cap = 1ULL << (i + WALK_RD);
                        fin = 1ULL << (i + JUMP_RD);
                        if ((cap & m.w) && !(fin & block)) {
                            //MOVE
                            k++;
                            td = m.b & ~(1ULL << i);
                            tl = m.w & ~cap;
                            tk = m.k & ~cap;
                            td = td | fin;
                            q.push(State(tl, td, tk));
                        }
                    }
                    if (tempi > 1 && tempj < 6) {
                        //--UP RIGHT--
                        cap = 1ULL << (i + WALK_RU);
                        fin = 1ULL << (i + JUMP_RU);
                        if ((cap & m.w) && !(fin & block)) {
                            //MOVE
                            k++;
                            td = m.b & ~(1ULL << i);
                            tl = m.w & ~cap;
                            tk = m.k & ~cap;
                            td = td | fin;
                            q.push(State(tl, td, tk));
                        }
                    }
                    if (tempi > 1 && tempj > 1) {
                        //--UP LEFT--
                        cap = 1ULL << (i + WALK_LU);
                        fin = 1ULL << (i + JUMP_LU);
                        if ((cap & m.w) && !(fin & block)) {
                            //MOVE
                            k++;
                            td = m.b & ~(1ULL << i);
                            tl = m.w & ~cap;
                            tk = m.k & ~cap;
                            td = td | fin;
                            q.push(State(tl, td, tk));
                        }
                    }
                    if (tempi < 6 && tempj>1) {
                        //DOWN LEFT
                        cap = 1ULL << (i + WALK_LD);
                        fin = 1ULL << (i + JUMP_LD);
                        if ((cap & m.w) && !(fin & block)) {
                            k++;
                            td = m.b & ~(1ULL << i);
                            tl = m.w & ~cap;
                            tk = m.k & ~cap;
                            td = td | fin;
                            q.push(State(tl, td, tk));
                            this->MakeMove(q.front());
                            this->UndoMove();
                        }
                    }
                    /*if i*/
                }
            }
            if (k == 0) { listm.push_back(this->moves[this->counter]); }
            this->UndoMove();
        }
        return listm;
    }
    list<State> GenWalksWhite() {
        list <State> listm;
        BitBoard wp = m.w&~m.k;
        BitBoard ep = ~(m.w | m.b);
        BitBoard wpl = (wp >> WALK_RD) & notHFile & ep;
        BitBoard wpr = (wp >> WALK_LD) & notAFile & ep;
        BitBoard temp;
        print(wpl);
        print(wpr);
        for (int i = 0; i < 64; i++, wpl >>= 1, wpr>>=1) {
            if (wpl & 0x01) {
                temp = (m.w | (1ULL << i)) & ~(1ULL << (i + WALK_RD));
                listm.push_back(State(temp, m.b, m.k));
            }
            if (wpr & 0x01) {
                temp = (m.w | (1ULL << i)) & ~(1ULL << (i + WALK_LD));
                listm.push_back(State(temp, m.b, m.k));
            }
        }
        return listm;
    }
    list<State> GenWalksBlack() {
        list <State> listm;
        BitBoard wp = m.b & ~m.k;
        BitBoard ep = ~(m.w | m.b);
        BitBoard wpl = (wp << WALK_LD) & notHFile & ep;
        BitBoard wpr = (wp << WALK_RD) & notAFile & ep;
        BitBoard temp;
        for (int i = 0; i < 64; wpl >>= 1, wpr >>= 1, i++) {
            if (wpl & 0x01) {
                temp = (wp | (1ULL << i)) & ~(1ULL << (i - WALK_RD));
                listm.push_back(State(m.w | (1ULL << i), 0, 0));
            }
            if (wpr & 0x01) {
                temp = (wp | (1ULL << i)) & ~(1ULL << (i - WALK_LD));
                listm.push_back(State(m.w | (1ULL << i), 0, 0));
            }
        }
        return listm;
    }
    list<State> LongestJumpBlack(list<State>& ml) {
        list<State> mx;
        int* ax = new int[ml.size()];
        cout << mx.size();
        int MaxSq=100000, temp;
        int i; list<State>::iterator it;
        for (it = ml.begin(), i = 0; it != ml.end(); it++, i++) {
            temp = std::popcount((*it).b);
            ax[i] = temp;
            if (temp < MaxSq) MaxSq = temp;
        }
        for (it = ml.begin(), i = 0; it != ml.end(); it++, i++) {
            if (ax[i] == MaxSq) mx.push_back(*it);
        }
        delete[] ax;
        return mx;
    }
    list<State> LongestJumpWhite(list<State>& ml) {
        list<State> mx;
        int* ax = new int[ml.size()];
        int MaxSq=1000000, temp;
        int i; list<State>::iterator it;
        for (it = ml.begin(), i = 0; it != ml.end(); it++, i++) {
            temp = std::popcount((*it).w);
            ax[i] = temp;
            if (temp < MaxSq) MaxSq = temp;
        }
        for (it = ml.begin(), i = 0; it != ml.end(); it++, i++) {
            if (ax[i] == MaxSq) mx.push_back(*it);
        }
        delete[] ax;
        return mx;
    }
    list<State> GenMoves() {
        if (counter % 2 == 0) {
            //cout << m.w << m.b;
            list<State> ml = GenJumpsWhite();
            if (ml.size() == 1) {
                if (ml.front() == m) { return GenWalksWhite(); }
            }
            return LongestJumpWhite(ml);
        }
        list<State> ml = GenJumpsBlack();
        if (ml.size() == 1) {
            if (ml.front() == m) { return GenWalksBlack(); }
        }
        return LongestJumpBlack(ml);
    }
    static void print(State m) {
        BitBoard k = m.OrAll();
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                std::cout << Get(k, i * 8 + j);
            }
            cout << '\n';
        }
        cout << "----------------------------------\n";
    }
    Board() {}
    ~Board() {}
};
int main()
{
    Board b = Board();
    b.FromArray(new int[64]{
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 2,
                0, 0, 1, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 1, 0, 1, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 
        });
    //b.m.w=1ULL<<20;
    Board::print(b.m);
    b.counter++;
    auto x = b.GenMoves();
    cout << x.size() << '\n';
    for (auto it = x.begin(); it != x.end(); ++it) {
        b.MakeMove(*it);
        Board::print(b.m);
        b.UndoMove();
    }

    return 0;
}
