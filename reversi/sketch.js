const SIZE = 8,
    WIDTH = HEIGHT = 700,
    CIRCLED = WIDTH / SIZE;

function Move(x, y, swaps) {
    this.x = x;
    this.y = y;
    this.swaps = swaps;
}

function Board() {
    this.arr = [];
    this.moves = new Array(1000);
    this.counter = 0;
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
    this.MakeMove = (move) => {
        this.arr[move.x][move.y] = this.counter % 2 + 1;
        this.moves[this.counter++] = move;
        for (let i of move.swaps) {
            for (let j of i) {
                this.arr[j[0]][j[1]] = 3 - this.arr[j[0]][j[1]];
            }
        }
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
        let gswaps = [];
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
                    gswaps.push(swaps);
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
                    Look1(i, j, this.arr, inc, fsf);
                    Look1(i, j, this.arr, fsf, inc);
                    Look1(i, j, this.arr, dec, fsf);
                    Look1(i, j, this.arr, fsf, dec);
                    Look1(i, j, this.arr, dec, dec);
                    Look1(i, j, this.arr, inc, inc);
                    Look1(i, j, this.arr, inc, dec);
                    Look1(i, j, this.arr, dec, inc);
                    //diagonal \/
                }
            }
        }
        return gswaps;
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
let b;

function setup() {
    createCanvas(WIDTH, HEIGHT);
    b = new Board();
}

function draw() {
    background(0, 190, 0);
    translate(-CIRCLED / 2, -CIRCLED / 2);

    b.show();
}