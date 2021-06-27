const cnv = document.getElementById("canvas1");
const ctx = cnv.getContext("2d");
const BIRD_POSITIONX = 80;
const BIRD_RADIUS = 25;
document.body.style.overflow = "hidden"; //REMOVE SCROLLING
const WIDTH = cnv.width = 800;
const HEIGHT = cnv.height = 800;
const GRAVITY = 0.6;
const ACCELERATION = -18;
const KOFF = 10;
const PIPE_SPAWN = 190;
const PIPE_WIDTH = 30;
const PIPE_SPEED = -2;
const BIRDS = 500;
let EVALUTION_SPEED = 10;