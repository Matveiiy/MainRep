let step = (x) => {
    return 1 / (1 + Math.exp(-x));
}

function nextGeneration() {
    pipes = [];
    birds[0] = bestbird.copy();
    birds[1] = new Bird();
    for (let i = 2; i < BIRDS; i++) {
        birds[i] = bestbird.copy();
        birds[i].n.mutate(0.15);
    }
    frameCount = 0;
}
let frameCount = 0;
let pipes = [];
let birds = [];
for (let i = 0; i < BIRDS; i++) birds[i] = new Bird();

function CalcClosest() {
    let maxd = Infinity;
    let c = null;
    if (pipes.length == 0) throw 1;
    for (let i = 0; i < pipes.length; i++) {
        let d = pipes[i].x - BIRD_POSITIONX;
        if (d < maxd && d > -BIRD_RADIUS) {
            maxd = d;
            c = pipes[i];
        }
    }
    return c;
}
let k = null;
let bestbird;
let label;
//document.addEventListener('keydown', (event) => { b.jump(); });
let loop = () => {
    EVALUTION_SPEED = document.getElementsByTagName("input")[0].value;
    for (let ph = 0; ph < EVALUTION_SPEED; ph++) {
        for (let p = pipes.length - 1; p >= 0; p--) {
            pipes[p].update();
            for (let j = birds.length - 1; j >= 0; j--) {
                if (pipes[p].hits(birds[j])) {
                    bestbird = birds[j];
                    birds.splice(j, 1);
                }
            }
            if (pipes[p].x < -PIPE_WIDTH) pipes.splice(p, 1);
        }
        if (birds.length == 0) nextGeneration();
        if (frameCount % PIPE_SPAWN == 0) pipes.push(new Pipe());
        k = CalcClosest();
        for (let i = 0; i < birds.length; i++) {
            if (frameCount % PIPE_SPAWN == 0) birds[i].score++;
            birds[i].update();
            birds[i].think(k);
        }
        frameCount++;

    }
    ctx.clearRect(0, 0, WIDTH, HEIGHT);
    for (let b of birds) {
        label.innerHTML = "Score: " + b.score;
        b.show();
    }
    for (let p of pipes) {
        p.show();
    }
    requestAnimationFrame(loop);
}
document.addEventListener("DOMContentLoaded", () => {
    label = document.getElementsByTagName("label")[0];
    label.innerHTML = "Score: ";
    loop();
});