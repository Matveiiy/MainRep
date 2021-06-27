function Pipe() {
    this.space = Math.random() * KOFF + HEIGHT / 3;
    this.top = Math.random() * (HEIGHT - this.space);
    this.bottom = this.top + this.space;
    this.x = WIDTH + 1;
    this.update = function() {
        this.x += PIPE_SPEED;
    }
    this.show = function() {
        ctx.beginPath();
        ctx.rect(this.x, 0, PIPE_WIDTH, this.top);
        ctx.rect(this.x, this.bottom, PIPE_WIDTH, HEIGHT);
        ctx.fill();
    }
    this.hits = function(bird) {
        function CheckOverlap(X1, Y1, X2, Y2) {
            let Xn = Math.max(X1, Math.min(BIRD_POSITIONX, X2));
            let Yn = Math.max(Y1, Math.min(bird.y, Y2));
            let Dx = Xn - BIRD_POSITIONX;
            let Dy = Yn - bird.y;
            return (Dx * Dx + Dy * Dy) <= BIRD_RADIUS * BIRD_RADIUS;
        }
        return CheckOverlap(this.x, 0, this.x + PIPE_WIDTH, this.top) || CheckOverlap(this.x, this.bottom, this.x + PIPE_WIDTH, HEIGHT);
    }
}