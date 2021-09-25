(function(){
  function loadShader(type, source) {
    const shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);
    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
      console.error('An error occurred compiling the shaders: ' + gl.getShaderInfoLog(shader));
      gl.deleteShader(shader);
      return null;
    }
  
    return shader;
  }
  this.initShaderProgram=function(vsSource, fsSource) {
    const vertexShader = loadShader(gl.VERTEX_SHADER, vsSource);
    const fragmentShader = loadShader(gl.FRAGMENT_SHADER, fsSource);
    const shaderProgram = gl.createProgram();
    gl.attachShader(shaderProgram, vertexShader);
    gl.attachShader(shaderProgram, fragmentShader);
    gl.linkProgram(shaderProgram);
    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
      console.warn('Unable to initialize the shader program: ' + gl.getProgramInfoLog(shaderProgram));
      return null;
    }
    return shaderProgram;
  }
  this.BindProgram=program=> gl.useProgram(program);
  this.canvas;
  this.gl;
  this.width;
  this.height;
  let current_program;
  this.initGL=function(canvasid) {
	canvas = document.getElementById(canvasid);
	if (!canvas) {
		console.log("canvas is not intialized");
	}
	gl = canvas.getContext('webgl2');
	if (!gl) {
		console.log("webgl is not initilized");
		return false;
	}
	return true;
  }
  this.CreateIBO=function(size_data, usage) {
	  let temp=gl.createBuffer();
	  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, temp);
	  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, size_data, usage);
	  return temp;
  }
  this.bindIBO=ibo => gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, ibo);
  this.IBOSubData=(data, offset, count)=>gl.bufferSubData(gl.ELEMENT_ARRAY_BUFFER, 0, data, offset, count);
  this.CreateVBO=function(size, usage){
	  let temp=gl.createBuffer();
	  gl.bindBuffer(gl.ARRAY_BUFFER, temp);
	  gl.bufferData(gl.ARRAY_BUFFER, size, usage);
	  return temp;
  } 
  this.VBOSubData=(data, offset, count)=>gl.bufferSubData(gl.ARRAY_BUFFER, 0, data, offset, count);
  this.bindVBO=vbo => gl.bindBuffer(gl.ARRAY_BUFFER, vbo);
  this.CreateVAO=function() {return gl.createVertexArray();}
  this.bindVAO=vao => gl.bindVertexArray(vao);
  this.setAttribVAO=function(attrib, number, type, normalise, size, offset) {
	gl.enableVertexAttribArray(attrib);
	  gl.vertexAttribPointer(
		attrib, 
		number, 
		type, 
		normalise,
		size, 
		offset 
	);
  }
  this.CreateTexture=function(path) {
	const texture = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D, texture);
	const internalFormat = gl.RGBA;
	const srcFormat = gl.RGBA;
	const pixel = new Uint8Array([0, 0, 255, 255]);  
	gl.texImage2D(gl.TEXTURE_2D, 0, internalFormat,
				  1, 1, 0, srcFormat, gl.UNSIGNED_BYTE,
				  pixel);
  
	const image = new Image();
	image.onload = function() {
	  gl.bindTexture(gl.TEXTURE_2D, texture);
	  gl.texImage2D(gl.TEXTURE_2D, 0, internalFormat,
		1, 1, 0, srcFormat, gl.UNSIGNED_BYTE,
		image);
	  gl.texImage2D(gl.TEXTURE_2D, 0, internalFormat,
					srcFormat, gl.UNSIGNED_BYTE, image);
  
	  if (isPowerOf2(image.width) && isPowerOf2(image.height)) {
		 gl.generateMipmap(gl.TEXTURE_2D);
	  } else {
		 // Размер не соответствует степени 2.
		 // Отключаем MIP'ы и устанавливаем натяжение по краям
		 gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
		 gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
		 gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
	  }
	};
	image.src = path;
	return texture;
}
function isPowerOf2(value) {
  return (value & (value - 1)) == 0;
}
})();
(function(){
	const _MAX_SPRITES=10;
	const _MAX_INDEX_LENGTH=_MAX_SPRITES*6;
	const _MAX_VERTEX_LENGTH=_MAX_SPRITES*36;
	this._Renderer2d=function() {
		this.init=function() {
			let vertexShaderText = `
			attribute vec3 pos;
			attribute vec4 color;
			attribute vec2 texcoords;
		
			varying vec2 out_texcoords;
			varying vec4 out_color;
			void main() {
				out_color=color;
				out_texcoords=texcoords;
				gl_Position=vec4(pos.x, pos.y, pos.z, 1);
			}
			`
			let fragmentShaderText =`
			precision mediump float;
			varying vec2 out_texcoords;
			varying vec4 out_color;
			uniform sampler2D tex;
			void main() {
				gl_FragColor=texture2D(tex, out_texcoords)*out_color;
			}
			`
			this.program=initShaderProgram(vertexShaderText, fragmentShaderText);BindProgram(this.program);
			this.vao=CreateVAO();bindVAO(this.vao);
			this.vbo=CreateVBO(_MAX_VERTEX_LENGTH, gl.DYNAMIC_DRAW);
			this.buffer=new Float32Array(_MAX_VERTEX_LENGTH);
			let index_data=new Uint16Array(_MAX_INDEX_LENGTH);
			this.size=0;
			this.drawcalls=0;
			this.active_texture=null;
			//--------------------------------------------
			let f=0;
			for (let k=0;k<_MAX_INDEX_LENGTH;k+=6, f+=4) {
				index_data[k]=f;
				index_data[k+1]=f+1;
				index_data[k+2]=f+2;
				index_data[k+3]=f+2;
				index_data[k+4]=f+3;
				index_data[k+5]=f;
			}
			let indices=CreateIBO(index_data, gl.STATIC_DRAW);
			index_data=null;
			f=null;
			//--------------------------------------------
			setAttribVAO(gl.getAttribLocation(this.program, "pos"),3, gl.FLOAT, gl.FALSE, 36, 0);
			setAttribVAO(gl.getAttribLocation(this.program, "color"),4, gl.FLOAT, gl.FALSE, 36, 12);
			setAttribVAO(gl.getAttribLocation(this.program, "texcoords"),2, gl.FLOAT, gl.FALSE, 36, 28);
			gl.uniform1i(gl.getUniformLocation(this.program, "tex"), 0);  // текстурный блок 0
			gl.clearColor(0, 0, 0, 1);
			bindVAO(null);bindVBO(null);bindIBO(null);
			
		}
		this.Begin=function() {
			this.size=0;this.drawcalls=0;
			gl.clear(gl.COLOR_BUFFER_BIT);
			bindVAO(this.vao);
			bindVBO(this.vbo);
		}
		this.DrawQuad=function(pos, size, color, texture) {
			if (this.active_texture!=texture) {
				this.End();
				this.active_texture=texture;
			}
			this.buffer[this.size+0]=pos.x-size.x/2.0;
			this.buffer[this.size+1]=pos.y-size.y/2.0;
			this.buffer[this.size+2]=pos.z;
			this.buffer[this.size+3]=color.r;
			this.buffer[this.size+4]=color.g;
			this.buffer[this.size+5]=color.b;
			this.buffer[this.size+6]=color.a;
			this.buffer[this.size+7]=0;
			this.buffer[this.size+8]=0;
			//--------------------------------
			this.buffer[this.size+9]=pos.x-size.x/2.0;
			this.buffer[this.size+10]=pos.y+size.y/2.0;
			this.buffer[this.size+11]=pos.z;
			this.buffer[this.size+12]=color.r;
			this.buffer[this.size+13]=color.g;
			this.buffer[this.size+14]=color.b;
			this.buffer[this.size+15]=color.a;
			this.buffer[this.size+16]=0;
			this.buffer[this.size+17]=1;
			//--------------------------------
			this.buffer[this.size+18]=pos.x+size.x/2.0;
			this.buffer[this.size+19]=pos.y+size.y/2.0;
			this.buffer[this.size+20]=pos.z;
			this.buffer[this.size+21]=color.r;
			this.buffer[this.size+22]=color.g;
			this.buffer[this.size+23]=color.b;
			this.buffer[this.size+24]=color.a;
			this.buffer[this.size+25]=1;
			this.buffer[this.size+26]=1;
			//--------------------------------
			this.buffer[this.size+27]=pos.x+size.x/2.0;
			this.buffer[this.size+28]=pos.y-size.y/2.0;
			this.buffer[this.size+29]=pos.z;
			this.buffer[this.size+30]=color.r;
			this.buffer[this.size+31]=color.g;
			this.buffer[this.size+32]=color.b;
			this.buffer[this.size+33]=color.a;
			this.buffer[this.size+34]=1;
			this.buffer[this.size+35]=0;
			this.size+=36;
		}
		this.End=function() {
			if (this.size==0) return;
			VBOSubData(this.buffer, 0, this.size); 
			gl.activeTexture(gl.TEXTURE0);
			gl.bindTexture(gl.TEXTURE_2D, this.active_texture);
			gl.drawElements(gl.TRIANGLES, this.size/36*6, gl.UNSIGNED_SHORT, 0);
			this.size=0;
			++this.drawcalls;
		}
		this.init();
	}
})();
//----------------------
var InitDemo = function () {
	if (!initGL("canvas1")) {alert("WEBGL initialization error");return;}
	let renderer=new _Renderer2d();
	let k=CreateTexture("spr1.png");
	let j=CreateTexture("spr2.png");
	let _loop=function() {
		renderer.Begin();
		renderer.DrawQuad({x:0.5, y:0.5, z:0}, {x:1, y:1}, {r:1, g:1, b:1, a:1}, k);
		renderer.DrawQuad({x:-0.5, y:-0.5, z:0}, {x:1, y:1}, {r:1, g:1, b:1, a:1}, j);
		/*
		renderer.DrawRenderable(k, [
			0, 0, 0, 1, 1, 1, 1, 0, 0,
			0, 1, 0, 1, 1, 1, 1, 0, 1,
			1, 1, 0, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 1, 1, 1, 1, 1, 0
		]);
		renderer.DrawRenderable(j, [
			0, 0,  0,0.1, 1, 1, 1, 0, 0,
			0, -1, 0,1, 1, 1, 1, 0, 1,
			-1, -1,0, 1, 1, 1, 1, 1, 1,
			-1, 0, 0,1, 1, 1, 1, 1, 0
		]);
		*/
		renderer.End();
		requestAnimationFrame(_loop);
	}
	_loop();
};
document.addEventListener("DOMContentLoaded",  InitDemo);