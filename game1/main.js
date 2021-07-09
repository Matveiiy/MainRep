let tmaps=[];
let images=[];
function AddTileMap(data) {
    let ind, k, s;
    for (let k=0;k<data.tilesets.length;k++) {
        k=0, ind=0;
        GetFile("assests/"+data.tilesets[k].source, (text)=>{
            ind=text.indexOf(" id=");
            while(ind!=-1) {
                ind+=5;
                s="";
                for (;text[ind]!='"';ind++) {
                    s+=text[ind];
                }
                let k=Number.parseInt(s);
                images[s]=new Image();
                ind=text.indexOf("source", ind)+8;
                s="";
                for (;text[ind]!='"';ind++) {
                    s+=text[ind];
                }
                console.log(s);
                images[k].src=s;
                ind=text.indexOf(" id=", ind);
            }
        })
    }
    tmaps.push(data);
}
let t;
function preload() {
    GetFile("assests/new.json", (data)=>{
        AddTileMap(JSON.parse(data));
    })
}
function setup() {
    createCanvas(300, 300);
}
function loop() {

}