const
    WIDTH = 500,
    HEIGHT = 500,
    SNAKE_WIDTH = 10,
    SNAKE_HEIGHT = 10,
    SNAKE_STARTX = 90,
    SNAKE_STARTY = 90,
    SNAKE_SPEED = 11,
    SNAKE_DELAY = 5,
    FOOD_RADIUS = 5,
    SNAKES = 10;
class Food {
    constructor() {
        this.pos = createVector(0, 0);
        this.pos.x = random(0, WIDTH);
        this.pos.y = random(0, HEIGHT);
    }
}

function step(x) {
    return 1 / (1 + Math.exp(-x));
}

function Snake() {
    this.Update = function() {
        if (this.CollideCircle(this.arr[0].x, this.arr[0].y, f.pos.x, f.pos.y)) {
            this.score++;
            this.GenFood();
            this.arr.push(createVector(this.arr[this.arr.length - 1].x, this.arr[this.arr.length - 1].y));
        }
        for (let i = this.arr.length - 1; i > 0; i--) {
            this.arr[i].x = this.arr[i - 1].x;
            this.arr[i].y = this.arr[i - 1].y;
        }
        this.arr[0].x += this.vel.x;
        this.arr[0].y += this.vel.y;
    }
    this.IsDead = function() {
        if (this.arr[0].x < 0 ||
            this.arr[0].x + SNAKE_WIDTH > WIDTH ||
            this.arr[0].y < 0 ||
            this.arr[0].y + SNAKE_HEIGHT > HEIGHT) return true;

        for (let i = 1; i < this.arr.length; i++) {
            if (this.CollideRects(this.arr[0].x, this.arr[0].y, this.arr[i].x, this.arr[i].y)) {
                //print(this.arr.get(0).x, this.arr.get(0).y, this.arr.get(i).x, this.arr.get(i).y);
                return true;
            }
        }
        return false;
    }
    this.draw = function() {
        for (let i = 0; i < this.arr.length; i++) {
            rect(this.arr[i].x, this.arr[i].y, SNAKE_WIDTH, SNAKE_HEIGHT);
        }
        ellipse(f.pos.x, f.pos.y, FOOD_RADIUS, FOOD_RADIUS);
    }
    this.CollideRects = function(x1, y1, x2, y2) {
        if (x1 >= x2 + SNAKE_WIDTH || x2 >= x1 + SNAKE_WIDTH) return false;
        if (y1 >= y2 + SNAKE_HEIGHT || y2 >= y1 + SNAKE_HEIGHT) return false;
        return true;
    }

    this.CollideCircle = function(x1, y1, xc, yc) {
        let xn = max(x1, min(xc, x1 + SNAKE_WIDTH));
        let yn = max(y1, min(yc, y1 + SNAKE_HEIGHT));

        let Dx = xn - xc;
        let Dy = yn - yc;
        return (Dx * Dx + Dy * Dy) <= FOOD_RADIUS * FOOD_RADIUS;
    }
    this.GenFood = function() {
        f = new Food();
    }
    this.think = function() {
        let output = this.brain.feedforward([
            [this.arr[0].x / WIDTH, this.arr[0].y / HEIGHT, f.pos.x / WIDTH, f.pos.y / HEIGHT]
        ], step)[0];
        let xa = math.max(output[0], output[1]);
        let ya = math.max(output[2], output[3]);
        if (xa > ya) {
            if (this.vel.y == 0) return;
            this.vel.y = 0;
            if (output[0] > output[1]) {
                this.vel.x = SNAKE_SPEED;
                print("right");
            } else {
                this.vel.x = -SNAKE_SPEED;
                print("left")
            }
        } else {
            if (this.vel.x == 0) return;
            this.vel.x = 0;
            if (output[2] > output[3]) {
                this.vel.y = SNAKE_SPEED;
                print("down")
            } else {
                this.vel.y = -SNAKE_SPEED;
                print("up")
            }
        }
    }
    this.vel = createVector();
    this.f = new Food();
    this.score = 3;
    this.arr = [];
    this.brain = new NeuralNetwork([4, 8, 4]);

    this.GenFood();
    this.vel = createVector(SNAKE_SPEED, 0);
    this.arr = [];
    this.arr.push(createVector(SNAKE_STARTX, SNAKE_STARTY));
    this.arr.push(createVector(SNAKE_STARTX - SNAKE_SPEED, SNAKE_STARTY));
    this.arr.push(createVector(SNAKE_STARTX - SNAKE_SPEED, SNAKE_STARTY));

}
let snakes = [];
let counter = 0;

function setup() {
    createCanvas(500, 500);
    for (let i = 0; i < SNAKES; i++) {
        snakes.push(new Snake());
    }
}

function draw() {
    counter++;
    background(0);
    fill(255);
    noStroke();
    //textSize(32);
    //text(s.score, 10, 30);
    if (counter % SNAKE_DELAY == 0) {
        for (let i = snakes.length - 1; i > -1; i--) {
            snakes[i].Update();
            snakes[i].think();
            if (snakes[i].IsDead()) {
                print("You looser!");
                //noLoop();
                snakes.splice(i, 1);
            }
        }
    }
    for (let s of snakes) {
        s.draw();
    }
}

function keyPressed() {
    //if (keyCode == UP_ARROW && s.vel.y == 0) {
    //    s.vel.y = -SNAKE_SPEED;
    //    s.vel.x = 0;
    //} else if (keyCode == DOWN_ARROW && s.vel.y == 0) {
    //    s.vel.y = SNAKE_SPEED;
    //    s.vel.x = 0;
    //} else if (keyCode == RIGHT_ARROW && s.vel.x == 0) {
    //    s.vel.x = SNAKE_SPEED;
    //    s.vel.y = 0;
    //} else if (keyCode == LEFT_ARROW && s.vel.x == 0) {
    //    s.vel.x = -SNAKE_SPEED;
    //    s.vel.y = 0;
    //}
}