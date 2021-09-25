let text;
preload=()=>{
    //text=CreateTexture("spr1.png");
}
onupdate=function(){
    renderer.DrawRenderable(null, text, [
        -0.5, -0.5, 1, 1, 1, 1.0, 
        -0.5, 0.5,  1, 1, 1, 1.0, 
        0.5, 0.5,   1, 1, 1, 1.0, 
        0.5, -0.50, 1, 1, 1, 1.0, 
    ])
    renderer.DrawRenderable(null, text, [
        -1.5, -1.5, 1, 1, 1, 1.0, 
        -1.5, -0.5, 1, 1, 1, 1.0, 
        -0.5, -0.5, 1, 1, 1, 1.0, 
        -0.5, -1.50,1, 1, 1, 1.0, 
    ])
}