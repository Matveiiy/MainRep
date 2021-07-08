const SIZE = 8,
    WIDTH = HEIGHT = 400,
    CIRCLED = WIDTH / SIZE;

function Move(x, y, swaps) {
    this.x = x;
    this.y = y;
    this.swaps = swaps||[];
}
function StaticEval(score) {
    this.score=score;
    this.bestm=null;
}
function Board() {
    this.moves = new Array(1000);
    this.counter = 0;
    this.arr = [];
    for (let i = 0; i < SIZE + 2; i++) {
        this.arr[i] = [];
        for (let j = 0; j < SIZE + 2; j++)
            this.arr[i][j] = 4;
    }
    for (let i = 1; i <= SIZE; i++) {
        for (let j = 1; j <= SIZE; j++) {
            this.arr[i][j] = 0;
        }
    }
    this.arr[4][4] = 1;
    this.arr[5][4] = 2;
    this.arr[4][5] = 2;
    this.arr[5][5] = 1;
    this.arr[SIZE+2]=[];
    this.arr[SIZE+2][SIZE+2]=0;
    //FUNCTIONS -\/-
    this.MakeMove = (move) => {
        this.arr[move.x][move.y] = this.counter % 2 + 1;
        this.moves[this.counter++] = move;
        for (let i of move.swaps) {
            for (let j of i) {
                this.arr[j[0]][j[1]] = 3 - this.arr[j[0]][j[1]];
            }
        }
    }
    this.MakeSafeMove=(move) => {
        let m=move;
        Look1 = (i, j, arr, funci, funcj) => {
            let swaps = [];
            i = funci(i);
            j = funcj(j);
            if (arr[i][j] == num) return;
            while (arr[i][j] != 0 && arr[i][j] != 4) {
                if (arr[i][j] == num) {
                    m.swaps.push(swaps);
                    break;
                }
                if (arr[i][j] == opp)
                    swaps.push([i, j]);
                i = funci(i);
                j = funcj(j);
            }
        }
        let num = 1 + this.counter % 2;
        let opp = 2 - this.counter % 2;
        
        let fsf = x => x;
        let inc = x => x + 1;
        let dec = x => x - 1;
        if (this.arr[move.x][move.y]!=0) return;
        Look1(move.x, move.y, this.arr, inc, fsf);
        Look1(move.x, move.y, this.arr, fsf, inc);
        Look1(move.x, move.y, this.arr, dec, fsf);
        Look1(move.x, move.y, this.arr, fsf, dec);
        Look1(move.x, move.y, this.arr, dec, dec);
        Look1(move.x, move.y, this.arr, inc, inc);
        Look1(move.x, move.y, this.arr, inc, dec);
        Look1(move.x, move.y, this.arr, dec, inc);
        if (m.swaps.length!=0) {
            this.MakeMove(m);
            return true; 
        }
        return false;
    }
    this.UndoMove = () => {
        this.counter--;
        this.arr[this.moves[this.counter].x][this.moves[this.counter].y] = 0;
        for (let i of this.moves[this.counter].swaps) {
            for (let j of i) {
                this.arr[j[0]][j[1]] = 3 - this.arr[j[0]][j[1]];
            }
        }
    }
    this.GenMoves = () => {
        let listmoves = [];
        let num = 1 + this.counter % 2;
        let opp = 2 - this.counter % 2;
        let m;
        let fsf = x => x;
        let inc = x => x + 1;
        let dec = x => x - 1;
        Look1 = (i, j, arr, funci, funcj) => {
            let swaps = [];
            i = funci(i);
            j = funcj(j);
            if (arr[i][j] == num) return;
            while (arr[i][j] != 0 && arr[i][j] != 4) {
                if (arr[i][j] == num) {
                    m.swaps.push(swaps);
                    break;
                }
                if (arr[i][j] == opp)
                    swaps.push([i, j]);
                i = funci(i);
                j = funcj(j);
            }
        }
        for (let i = 1; i <= SIZE; i++) {
            for (let j = 1; j <= SIZE; j++) {
                if (this.arr[i][j] == 0) {
                    m=new Move(i, j, []);
                    Look1(i, j, this.arr, inc, fsf);
                    Look1(i, j, this.arr, fsf, inc);
                    Look1(i, j, this.arr, dec, fsf);
                    Look1(i, j, this.arr, fsf, dec);
                    Look1(i, j, this.arr, dec, dec);
                    Look1(i, j, this.arr, inc, inc);
                    Look1(i, j, this.arr, inc, dec);
                    Look1(i, j, this.arr, dec, inc);
                    if (m.swaps.length>0) listmoves.push(m);
                    //diagonal \/
                }
            }
        }
        if (listmoves.length==0) {
            return null;
        }
        return listmoves;
    }
    //skip move
    this.pass=()=>{
        this.MakeMove(new Move(SIZE+2, SIZE+2, [[[SIZE+2, SIZE+2]]]));
    }
    this.show = () => {
        stroke(0, 0, 0);
        for (let i = 1; i <= SIZE; i++) {
            line(0, i * CIRCLED - CIRCLED / 2, WIDTH + 100, i * CIRCLED - CIRCLED / 2);
            line(i * CIRCLED - CIRCLED / 2, 0, i * CIRCLED - CIRCLED / 2, HEIGHT + 100);
            for (let j = 1; j <= SIZE; j++) {
                if (this.arr[i][j] == 1) {
                    fill(255);
                    circle(i * CIRCLED, j * CIRCLED, CIRCLED);
                } else if (this.arr[i][j] == 2) {
                    fill(0);
                    circle(i * CIRCLED, j * CIRCLED, CIRCLED);
                }
            }
        }
    }
}
function Game() {
    this.board=new Board();
    this.gomax=(depth)=>{
        if (depth==0) return this.evaluation();
        let x=this.board.GenMoves();
        if (x==null) return this.gomin(depth-1);
        let score, mscore=new StaticEval(-Infinity);
        for (let k of x) {
            this.board.MakeMove(k);
            score=this.gomin(depth-1);
            if (score.score>mscore.score) {
                mscore.score=score.score;
                mscore.bestm=k;
            }
            this.board.UndoMove();
        }
        return mscore;
    }
    this.gomin=(depth)=>{
        if (depth==0) return this.evaluation();
        let x=this.board.GenMoves();
        if (x==null) return this.gomax(depth-1);
        let score, mscore=new StaticEval(Infinity);
        for (let k of x) {
            this.board.MakeMove(k);
            score=this.gomax(depth-1);
            if (score.score<mscore.score) {
                mscore.score=score.score;
                mscore.bestm=k;
            }
            this.board.UndoMove();
        }
        return mscore;
    }
    this.countmaterial=()=>{
        let k1=0, k2=0;
        for (let i = 1; i <= SIZE; i++) {
            for (let j = 1; j <= SIZE; j++) {
                if (this.board.arr[i][j] == 1) 
                    k1++;
                else if (this.board.arr[i][j] == 2)
                    k2++; 
            }
        }
        return {k1:k1, k2:k2};
    }
    this.evaluation=()=>{
        let k1=0, k2=0;
        for (let i = 1; i <= SIZE; i++) {
            for (let j = 1; j <= SIZE; j++) {
                if (this.board.arr[i][j] == 1) 
                    k1++;
                else if (this.board.arr[i][j] == 2)
                    k2++; 
            }
        }
        return new StaticEval(k1-k2);
    }
    this.think=function(depth) {
        return this.gomin(depth);
    }
}
let b;
let slider;
let x;
function setup() {
    createCanvas(WIDTH+10, HEIGHT);
    b = new Game();
    slider=createSlider(1, 10, 2);
}

function draw() {
    background(0, 190, 0);
    translate(-CIRCLED / 2, -CIRCLED / 2);
    b.board.show();
    fill(0);
}
function ComputerMove() {
    let x=b.think(slider.value());
    if (x.bestm==null) return false;
    b.board.MakeMove(x.bestm);
    print(x);
    return true;
}
function mousePressed() {
    if (b.board.MakeSafeMove(new Move(Math.floor(mouseX/CIRCLED)+1, Math.floor(mouseY/CIRCLED)+1))) {
        if (ComputerMove()==false) {
            b.board.pass();
            if (b.board.GenMoves()==null) {
                let s="Game Ended: \n";
                let k3=b.countmaterial();
                s+="Black: "+k3.k2.toString()+"\n";
                s+="White: "+k3.k1.toString();
                alert(s);
                //noLoop();
                return;
            }
        }
        else {
            while(b.board.GenMoves()==null) {
                b.board.pass();
                if (ComputerMove()==false) {
                    let s="Game Ended: \n";
                    let k3=b.countmaterial();
                    s+="Black: "+k3.k2.toString()+"\n";
                    s+="White: "+k3.k1.toString();
                    alert(s);
                    //noLoop();
                    return;
                }
            }
        }
    }
}
