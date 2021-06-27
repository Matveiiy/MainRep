let Bird = function() {
    this.y = HEIGHT / 2;
    this.vel = 0;
    this.score = 0;
    this.n = new NeuralNetwork([5, 8, 1]);

    this.jump = function() { this.vel += ACCELERATION; }
    this.think = function(closest) {
        let output = this.n.feedforward([
            [this.vel / 5.0, this.y / HEIGHT, closest.x / WIDTH, closest.top / HEIGHT, closest.bottom / HEIGHT]
        ], step);
        if (output[0] > 0.5) this.jump();
    }
    this.update = function() {
        this.vel += GRAVITY;
        this.y += this.vel;
        this.vel *= 0.9;
        if (this.y < 0) {
            this.y = 0;
            this.vel = 0;
        }
        if (this.y > HEIGHT) { this.y = HEIGHT; }
    }
    this.show = function() {
        ctx.beginPath();
        ctx.arc(BIRD_POSITIONX, this.y, BIRD_RADIUS, 0, 2 * Math.PI);
        ctx.fill();
    }
    this.copy = function() {
        let t = new Bird();
        t.y = this.y;
        t.vel = this.vel;
        t.n = this.n.copy();
        return t;
    }
}