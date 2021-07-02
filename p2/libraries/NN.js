const MUT_RANGEB = -0.1;
const MUT_RANGET = 0.1;
const MUT_RANGEX = MUT_RANGET - MUT_RANGEB;

function NeuralNetwork(arr) {
    this.layers = [];

    function CreateLayer(before, now) {
        let b = []
        for (let i = 0; i < before + 1; i++) {
            b[i] = [];
            for (let j = 0; j < now; j++) {
                b[i][j] = Math.random() * 2 - 1;
            }
        }
        return b;
    }
    if (arr instanceof NeuralNetwork) {
        for (let i = 0; i < arr.layers.length; i++) {
            this.layers[i] = [];
            for (let k = 0; k < arr.layers[i].length; k++) {
                this.layers[i][k] = [];
                for (let j = 0; j < arr.layers[i][k].length; j++) {
                    this.layers[i][k][j] = arr.layers[i][k][j];
                }
            }
        }
        return;
    }
    for (let i = 1; i < arr.length; i++) {
        this.layers[i - 1] = CreateLayer(arr[i - 1], arr[i]);
    }

    this.feedforward = function(inputs, func) {
        for (let l of this.layers) {
            inputs[0].push(1);
            inputs = math.map(math.multiply(inputs, l), func);
        }
        return inputs;
    }

    this.copy = function() {
        let t = new NeuralNetwork(this);
        return t;
    }
    this.mutate = function(rate) {
        for (let i = 0; i < this.layers.length; i++) {
            for (let k = 0; k < this.layers[i].length; k++) {
                for (let j = 0; j < this.layers[i][k].length; j++) {
                    if (Math.random() > rate)
                        this.layers[i][k][j] += Math.random() * MUT_RANGEX + MUT_RANGEB;
                }
            }
        }
    }
}