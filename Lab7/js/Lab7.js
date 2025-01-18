let speed = 50;

let rotateX = false;
let rotateY = false;
let rotateZ = false;
let selectedShape = "cube";
let rotationX = 0;
let rotationY = 0;
let rotationZ = 0;
let prevX = 0;
let prevY = 0;
let prevZ = 0;

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
    const texture1 = loadTexture(gl, "cube");
    const texture2 = loadTexture(gl, "tetr");
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
    function render(now) {
      rotationX = rotateX ? rotationX + speed * 0.002 : rotationX;
      rotationY = rotateY ? rotationY + speed * 0.002 : rotationY;
      rotationZ = rotateZ ? rotationZ + speed * 0.002 : rotationZ;
      drawScene(gl, programInfo, buffers, rotationX,rotationY,rotationZ,prevX,prevY,prevZ,texture1,texture2,selectedShape);
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