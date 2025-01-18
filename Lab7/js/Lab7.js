import { initBuffers } from "./init-buffers.js";
import { drawScene } from "./draw-scene.js";

main();

let speed = 50;

let rotateX = false;
let rotateY = false;
let rotateZ = false;
let selectedShape = "cube";
let rotationX = 0;
let rotationY = 0;
let rotationZ = 0;

document.getElementById("rotateX").checked = false;
document.getElementById("rotateX").addEventListener("change", (event) => {
  rotateX = event.target.checked;
  console.log("Rotate X:", rotateX);
});
document.getElementById("rotateY").checked = false;
document.getElementById("rotateY").addEventListener("change", (event) => {
  rotateY = event.target.checked; 
  console.log("Rotate Y:", rotateY);
});
document.getElementById("rotateZ").checked = false;
document.getElementById("rotateZ").addEventListener("change", (event) => {
  rotateZ = event.target.checked;
  console.log("Rotate Z:", rotateZ);
});
document.getElementById("cube").checked = true;
document.getElementById("cube").addEventListener("change", (event) => {
  if (event.target.checked) {
      selectedShape = "cube";
      document.getElementById("tetrahedron").checked = false;
      console.log("Selected shape:", selectedShape);
  }
});
document.getElementById("tetrahedron").checked = false;
document.getElementById("tetrahedron").addEventListener("change", (event) => {
  if (event.target.checked) {
      selectedShape = "tetrahedron"; 
      document.getElementById("cube").checked = false;
      console.log("Selected shape:", selectedShape);
  }
});
document.getElementById("speedRange").value = speed;
document.getElementById("speedRange").addEventListener("input", (event) => {
  speed = event.target.value; // Update rotation speed
  console.log("Rotation speed:", speed);
});
function main() {
    const canvas = document.querySelector("#gl-canvas");
    canvas.width = canvas.clientWidth;
    canvas.height = canvas.clientHeight;
    const gl = canvas.getContext("webgl");
    if (gl === null) {
        alert(
        "Unable to initialize WebGL. Your browser or machine may not support it.",
        );
        return;
    }
    gl.viewport(0, 0, gl.drawingBufferWidth, gl.drawingBufferHeight);
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);
    const vsSource = `
      attribute vec4 aVertexPosition;
      attribute vec2 aTextureCoord;
      uniform mat4 uModelViewMatrix;
      uniform mat4 uProjectionMatrix;
      varying highp vec2 vTextureCoord;
      void main(void) {
        gl_Position = uProjectionMatrix * uModelViewMatrix * aVertexPosition;
        vTextureCoord = aTextureCoord;
      }
    `;
    const fsSource = `
      varying highp vec2 vTextureCoord;
      uniform sampler2D uSampler;
      void main(void) {
        gl_FragColor = texture2D(uSampler, vTextureCoord);
      }
    `;

    const shaderProgram = initShaderProgram(gl, vsSource, fsSource);
    const programInfo = {
      program: shaderProgram,
      attribLocations: {
        vertexPosition: gl.getAttribLocation(shaderProgram, "aVertexPosition"),
        textureCoord: gl.getAttribLocation(shaderProgram, "aTextureCoord"),
      },
      uniformLocations: {
        projectionMatrix: gl.getUniformLocation(shaderProgram, "uProjectionMatrix"),
        modelViewMatrix: gl.getUniformLocation(shaderProgram, "uModelViewMatrix"),
        uSampler: gl.getUniformLocation(shaderProgram, "uSampler"),
      },
    };    
    const buffers = initBuffers(gl);
    const texture = loadTexture(gl, "tekstura1.png");
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
    function render(now) {
      rotationX = rotateX ? rotationX + speed * 0.001 : rotationX;
      rotationY = rotateY ? rotationY + speed * 0.001 : rotationY;
      rotationZ = rotateZ ? rotationZ + speed * 0.001 : rotationZ;
      drawScene(gl, programInfo, buffers, rotationX,rotationY,rotationZ,texture,selectedShape);
      requestAnimationFrame(render);
    }
    requestAnimationFrame(render);
}
function initShaderProgram(gl, vsSource, fsSource) {
    const vertexShader = loadShader(gl, gl.VERTEX_SHADER, vsSource);
    const fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fsSource);
    const shaderProgram = gl.createProgram();
    gl.attachShader(shaderProgram, vertexShader);
    gl.attachShader(shaderProgram, fragmentShader);
    gl.linkProgram(shaderProgram);
    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
      alert(
        `Unable to initialize the shader program: ${gl.getProgramInfoLog(
          shaderProgram,
        )}`,
      );
      return null;
    }
    return shaderProgram;
}
function loadShader(gl, type, source) {
    const shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);
    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
      alert(
        `An error occurred compiling the shaders: ${gl.getShaderInfoLog(shader)}`,
      );
      gl.deleteShader(shader);
      return null;
    }  
    return shader;
}
function loadTexture(gl, url) {
  const texture = gl.createTexture();
  gl.bindTexture(gl.TEXTURE_2D, texture);
  const level = 0;
  const internalFormat = gl.RGBA;
  const width = 1;
  const height = 1;
  const border = 0;
  const srcFormat = gl.RGBA;
  const srcType = gl.UNSIGNED_BYTE;
  const pixel = new Uint8Array([0, 0, 255, 255]);
  gl.texImage2D(
    gl.TEXTURE_2D,
    level,
    internalFormat,
    width,
    height,
    border,
    srcFormat,
    srcType,
    pixel,
  );
  const image = new Image();
  image.onload = () => {
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texImage2D(
      gl.TEXTURE_2D,
      level,
      internalFormat,
      srcFormat,
      srcType,
      image,
    );
    if (isPowerOf2(image.width) && isPowerOf2(image.height)) {
      gl.generateMipmap(gl.TEXTURE_2D);
    } else {
      gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
      gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
      gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    }
  };
  image.src = url;
  return texture;
}
function isPowerOf2(value) {
  return (value & (value - 1)) === 0;
}