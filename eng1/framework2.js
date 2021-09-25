let gl, cnv;
(function() {
    //--------------------
//private:
function parseOBJ(text) {
    // because indices are base 1 let's just fill in the 0th data
    const objPositions = [[0, 0, 0]];
    const objTexcoords = [[0, 0]];
    const objNormals = [[0, 0, 0]];
  
    // same order as `f` indices
    const objVertexData = [
      objPositions,
      objTexcoords,
      objNormals,
    ];
  
    // same order as `f` indices
    let webglVertexData = [
      [],   // positions
      [],   // texcoords
      [],   // normals
    ];
  
    function newGeometry() {
      // If there is an existing geometry and it's
      // not empty then start a new one.
      if (geometry && geometry.data.position.length) {
        geometry = undefined;
      }
      setGeometry();
    }
  
    function addVertex(vert) {
      const ptn = vert.split('/');
      ptn.forEach((objIndexStr, i) => {
        if (!objIndexStr) {
          return;
        }
        const objIndex = parseInt(objIndexStr);
        const index = objIndex + (objIndex >= 0 ? 0 : objVertexData[i].length);
        webglVertexData[i].push(...objVertexData[i][index]);
      });
    }
  
    const keywords = {
      v(parts) {
        objPositions.push(parts.map(parseFloat));
      },
      vn(parts) {
        objNormals.push(parts.map(parseFloat));
      },
      vt(parts) {
        // should check for missing v and extra w?
        objTexcoords.push(parts.map(parseFloat));
      },
      f(parts) {
        const numTriangles = parts.length - 2;
        for (let tri = 0; tri < numTriangles; ++tri) {
          addVertex(parts[0]);
          addVertex(parts[tri + 1]);
          addVertex(parts[tri + 2]);
        }
      },
    };
  
    const keywordRE = /(\w*)(?: )*(.*)/;
    const lines = text.split('\n');
    for (let lineNo = 0; lineNo < lines.length; ++lineNo) {
      const line = lines[lineNo].trim();
      if (line === '' || line.startsWith('#')) {
        continue;
      }
      const m = keywordRE.exec(line);
      if (!m) {
        continue;
      }
      const [, keyword, unparsedArgs] = m;
      const parts = line.split(/\s+/).slice(1);
      const handler = keywords[keyword];
      if (!handler) {
        console.warn('unhandled keyword:', keyword);  // eslint-disable-line no-console
        continue;
      }
      handler(parts, unparsedArgs);
    }
  
    return {
      position: webglVertexData[0],
      texcoord: webglVertexData[1],
      normal: webglVertexData[2],
    };
  }
    function init_mesh(vertices, indices){
        return {
            data: 0
        }
    }
    function _loadShader(type, source) {
        const shader = gl.createShader(type);
        gl.shaderSource(shader, source);
        gl.compileShader(shader);
        if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
          alert('An error occurred compiling the shaders: ' + gl.getShaderInfoLog(shader));
          gl.deleteShader(shader);
          return null;
        }
        return shader;
    }
    let _is_running=true, _load_count=0;
    let _objs_to_delete=[];
    //--------------------
//public:
    this.quit=function(){_is_running=false;}
    this.resize=function(w, h) {cnv.width=w;cnv.height=h;gl.viewport(0, 0, w, h);}
    this.get_canvas=function(name){
        cnv=document.getElementById(name);
        try {gl=cnv.getContext("webgl2");}catch(e){};
        if (!gl) {
            alert("Unable to initialize WebGL. Your browser may not support it.")
            _extra_exit();
            return;
        }
    }
    this.loadimg=function(path) {
        let img=new Image();
        _load_count++;
        img.onload=function(e){
            _load_count--;
            if (_load_count==0) korify_run();
        }
        img.src=path;
        return img;
    }
    this.initFrameBuffer=function() {}//Not implemanted& no clean
    this.initRendererBuffer=function() {}//Not implemanted& no clean
    this.initTexture=function() {
        _objs_to_delete.push({data: 0,func: function() {gl.deleteTexture(this.data);}});
    }//Not implemanted but clean
    this.initVertexArray=function() {
        let vao=gl.createVertexArray();
        _objs_to_delete.push({
            data: vao,
            func: function() {gl.deleteVertexArray(this.data);}
        });
        return vao;
    }
    this.initBuffer=function() {
        let buffer=gl.createBuffer();
        _objs_to_delete.push({
            data: buffer,
            func: function() {gl.deleteBuffer(this.data);}
        });
        return buffer;
    } 
    this.initShaderProgram=function(vsSource, fsSource) {
        const vertexShader = _loadShader(gl.VERTEX_SHADER, vsSource);
        const fragmentShader = _loadShader(gl.FRAGMENT_SHADER, fsSource);
      
        const shaderProgram = gl.createProgram();
        gl.attachShader(shaderProgram, vertexShader);
        gl.attachShader(shaderProgram, fragmentShader);
        gl.linkProgram(shaderProgram);
      
        if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
          alert('Unable to initialize the shader program: ' + gl.getProgramInfoLog(shaderProgram));
          return null;
        }
        gl.useProgram(shaderProgram);
        gl.detachShader(shaderProgram, vertexShader);
        gl.detachShader(shaderProgram, fragmentShader);
        _objs_to_delete.push({
            data: shaderProgram,
            func: function () {
                console.log(this.data);gl.deleteProgram(this.data);
            }
        })
        return shaderProgram;
    }
    async function load() {
        const response = await fetch('obj.obj');  /* webglfundamentals: url */
        console.log(parseOBJ(await response.text()));
    }
    this.RendererNoTexture=function() {
        //load();
        //console.log(text);
        //debugger;
        this.draw=function() {
            gl.useProgram(this.program)
            gl.drawArrays(gl.TRIANGLES, 0, 3);
        }
        this.delete=function() {}
        if (program==undefined || program == null) {
            const vSource=`
            attribute vec3 pos;
            attribute vec3 normal;
            attribute vec3 vcolor;
            varying vec4 color;
            void main() {
                gl_Position = vec4(pos, 1);
                color=vcolor;
            }
            `;
            const fSource=`
            varying vec4 color;
            void main() {
                gl_FragColor = color;
            }
            `;
            this.program=initShaderProgram(vSource, fSource);
        }else this.program=program;
        this.vertexpos= gl.getAttribLocation(this.program, 'pos')
        //-----
        this.buffer=initBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([
            0, 0, 
            1, 1,
            0, 1
        ]), gl.STATIC_DRAW);
        
        this.vao=initVertexArray();
        gl.bindVertexArray(this.vao);
        gl.vertexAttribPointer(this.vertexpos, 2, gl.FLOAT, false, 2*4, 0);gl.enableVertexAttribArray(this.vertexpos);

    }
    this.Renderer2d=function(program){
        this.draw=function() {
            gl.useProgram(this.program)
            gl.drawArrays(gl.TRIANGLES, 0, 3);
        }
        this.delete=function() {}
        if (program==undefined || program == null) {
            const vSource=`
            attribute vec2 pos;
            void main() {
                gl_Position = vec4(pos, 0, 1);
            }
            `;
            const fSource=`
            void main() {
                gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
            }
            `;
            this.program=initShaderProgram(vSource, fSource);
        }else this.program=program;
        this.vertexpos= gl.getAttribLocation(this.program, 'pos')
        //-----
        this.buffer=initBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([
            0, 0,
            1, 1,
            0, 1
        ]), gl.STATIC_DRAW);
        
        this.vao=initVertexArray();
        gl.bindVertexArray(this.vao);
        gl.vertexAttribPointer(this.vertexpos, 2, gl.FLOAT, false, 2*4, 0);gl.enableVertexAttribArray(this.vertexpos);
    }
    this.coreCleanUp=function() {
        
    }
    function korify_run(e) {
        if (typeof setup=="undefined") {setup=()=>{};}
        if (typeof mainloop=="undefined") {mainloop=()=>{};}
        if (typeof destroy=="undefined") {destroy=()=>{};}
        setup();
        let _mainloop=function() {
            mainloop();
            if (_is_running==false) {
                for (let k of _objs_to_delete) {
                    k.func();
                }
                destroy();return;
            };
            requestAnimationFrame(_mainloop);
        }
        _mainloop()
    }
    document.addEventListener("DOMContentLoaded", korify_run);
})()