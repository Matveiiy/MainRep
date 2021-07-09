function Vector2(x, y){
    this.x=x;
    this.y=y;
}
function GetFile(path, success) {
    let xhr=new XMLHttpRequest();
    xhr.onreadystatechange = function() {
        if (xhr.readyState === XMLHttpRequest.DONE) {
            if (xhr.status === 200) {
                if (success) success(xhr.responseText);
            } else {
                console.log("error");
                console.log(xhr);
                return undefined;
            }
        }
    };
    xhr.open("GET", path, true);
    xhr.send();
}
//[private]
let noLoop, resize, createCanvas, fill, stroke, noStroke, noFill, rectf, drawimage, line;
let cnv, ctx, width, height;
document.addEventListener("DOMContentLoaded", ()=>{
    resize=(x, y)=>{width=cnv.width=x;height=cnv.height=y;};
    fill=s=>ctx.fillStyle=s;
    stroke=s=>ctx.strokeStyle=s;
    noStroke=()=>ctx.strokeStyle="";
    noFill=()=>ctx.fillStyle="";
    line=(x1, y1, x2, y2) =>{
        ctx.beginPath();       // Начинает новый путь
        ctx.moveTo(x1, y1);    // Передвигает перо в точку (30, 50)
        ctx.lineTo(x2, y2);  // Рисует линию до точки (150, 100)
        ctx.stroke(); 
    }
    rectf=(x, y, w, h)=>ctx.fillRect(x, y, w, h);
    createCanvas=(w, h)=>{
        cnv=document.createElement("canvas");
        cnv.id="mycanvas1";
        cnv.width=500;
        cnv.height=200;
        document.body.appendChild(cnv);
        //
        cnv=document.getElementById("mycanvas1");
        ctx=cnv.getContext("2d");
        resize(w||300, h||300);
    };
    noLoop=()=>{mainloop=()=>{};};
    //----------
    preload();
});
window.addEventListener("load", ()=>{
    let mainloop;
    if (typeof setup=="undefined") throw "setup is an important component";
    if (typeof loop == "undefined") mainloop=()=>{};
    else {
        mainloop=()=>{
            loop();
            requestAnimationFrame(mainloop);
        }
    }
    setup();
    mainloop();
})