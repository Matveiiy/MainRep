#include <iostream>
#include <queue>
#include <list>
#define SHIFT_RD 9
#define SHIFT_LD 7
#define SHIFT_RU -7
#define SHIFT_LU -9
//RIGHT=>notAFile
//LEFT=>notHFile
using namespace std;
#define BitBoard uint64_t
const BitBoard notAFile = 0xfefefefefefefefe; // ~0x0101010101010101
const BitBoard notHFile = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080
BitBoard MoveNW(BitBoard k) {
    return (k<<9)&notAFile;
}
class State{
    public:
    BitBoard b, w, k;
    BitBoard OrAll() {return w|b;}
    State(BitBoard B=0, BitBoard W=0,BitBoard K=0):b(B), w(W), k(K)   {}
};
// positve left, negative right shifts
const int shift[8] = {9, 1,-7,-8,-9,-1, 7, 8};
const int walks[4]={9, 7, -7, -9};
const int jumps[4]={18, 14, -14, -18};
class Board{

    public:
    State m;
    State moves[1000];
    int counter=0;
    void FromArray(int* arr) {
        for (int i=0;i<8;i++) {
            for (int j=0;j<8;j++) {
                if(arr[i*8+j]==1) {Set(m.w, i*8+j);}
                else if(arr[i*8+j]==2) {Set(m.b, i*8+j);}
                else if(arr[i*8+j]==3) {Set(m.w, i*8+j);Set(m.k, i*8+j);}
                else if(arr[i*8+j]==4) {Set(m.b, i*8+j);Set(m.k, i*8+j);}
            }
        }
    }
    void Set(BitBoard& number, int n) {number |= 1ULL << n;}
    bool Get(BitBoard number, int n) {return (number >> n) & 1U;}
    void MakeMove(State M) {m=M;moves[++counter]=M;}
    void UndoMove() {m=moves[--counter];}
    list<State> GenMovesWhite() {
        list<State> listm;
        queue<State> q;
        BitBoard tl, td, tk;
        BitBoard block, wp, cap, fin, tempi, tempj, k=0;
        State nextstate;
        q.push(m);
        while (!q.empty()) {
            this->MakeMove(q.front());
            q.pop();
            block=m.b|m.w;
            wp=m.w&~m.k;
            for(int i=0;i<64;i++) {
                if (Get(wp, i)) {
                    cout << "1\n";
                    tempj=i%8;
                    tempi=i/8;
                    if (tempi<6 && tempj<6) {
                        //--DOWN RIGHT--
                        cap=1ULL<<(i+SHIFT_RD);
                        fin=1ULL<<(i+SHIFT_RD);
                        if ((cap&m.b) && !(fin&block)) {
                            //MOVE
                            k++;
                            tl=m.w & ~(1ULL << i);
                            td=m.b & ~cap;
                            tk=m.k & ~cap;
                            tl=tl | fin;
                            q.push(State(tl, td, tk));
                        }
                    }
                    if (tempi>1 && tempj<6) {
                        //--UP RIGHT--
                        cap=1ULL<<(i+SHIFT_RU);
                        fin=1ULL<<(i+SHIFT_RU);
                        if ((cap&m.b) && !(fin&block)) {
                            //MOVE
                            k++;
                            tl=m.w & ~(1ULL << i);
                            td=m.b & ~cap;
                            tk=m.k & ~cap;
                            tl=tl | fin;
                            q.push(State(tl, td, tk));
                        }
                    }
                    if (tempi>1 && tempj>1) {
                        //--UP LEFT--
                        cap=1ULL<<(i+SHIFT_LU);
                        fin=1ULL<<(i+SHIFT_LU);
                        if ((cap&m.b) && !(fin&block)) {
                            //MOVE
                            k++;
                            tl=m.w & ~(1ULL << i);
                            td=m.b & ~cap;
                            tk=m.k & ~cap;
                            tl=tl | fin;
                            q.push(State(tl, td, tk));
                        }
                    }
                    if (tempi<6 && tempj>1) {
                        //DOWN LEFT
                        cap=1ULL<<(i+SHIFT_LD);
                        fin=1ULL<<(i+SHIFT_LD);
                        if ((cap&m.b) && !(fin&block)) {
                            //MOVE
                            k++;
                            tl=m.w & ~(1ULL << i);
                            td=m.b & ~cap;
                            tk=m.k & ~cap;
                            tl=tl | fin;
                            q.push(State(tl, td, tk));
                        }
                    }
                    if (k==0) {listm.push_back(this->moves[this->counter]);}
                    /*if i*/
                }
            }
            cout << 2;
            this->UndoMove();
        }
    }
    void print() {
        BitBoard k=m.OrAll();
        for (int i=0;i<8;i++) {
            for (int j=0;j<8;j++) {
                std::cout << Get(k, i*8+j);
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
    Board b=Board();
    b.FromArray(new int[64]{
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 1,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                });
    //b.m.w=1ULL<<20;
    b.print();
    auto x=b.GenMovesWhite();
    for (auto it=x.begin();it!=x.end();++it) {
        b.MakeMove(*it);
        b.print();
        b.UndoMove();
    }
    //\
    //_\|
    //b.m.w+=27;
    //b.print();
    return 0;
}
