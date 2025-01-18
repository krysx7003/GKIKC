function initBuffers(gl) {
    const cubeBuffers = {
        position: initCubePositionBuffer(gl),
        textureCoord: initCubeTextureBuffer(gl),
        indices: initCubeIndexBuffer(gl),
    }
    const tetrBuffers ={
        position: initTetrPositionBuffer(gl),
        textureCoord: initTetrTextureBuffer(gl),
        indices: initTetrIndexBuffer(gl),
    }
    return {
        cube: cubeBuffers,
        tetr: tetrBuffers,
    };
}
function initCubePositionBuffer(gl) {
    const positionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
    const positions = [
        -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,         // front
        -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0,     // back
        -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0,         // top
        -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0,     // bottom
        1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0,         // right
      ];  
    gl.bufferData(
        gl.ARRAY_BUFFER, 
        new Float32Array(positions), 
        gl.STATIC_DRAW
    );
    return positionBuffer;
}
function initCubeIndexBuffer(gl) {
    const indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
    const indices = [
      0,1,2,0,2,3,          // front
      4,5,6,4,6,7,          // back
      8,9,10,8,10,11,       // top
      12,13,14,12,14,15,    // bottom
      16,17,18,16,18,19,    // right
    ];
    gl.bufferData(
      gl.ELEMENT_ARRAY_BUFFER,
      new Uint16Array(indices),
      gl.STATIC_DRAW,
    );
    return indexBuffer;
  }
function initCubeTextureBuffer(gl) {
    const textureCoordBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, textureCoordBuffer);
    const textureCoordinates = [
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,   // front
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,   // back
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,   // top
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,   // bottom
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,   // right
    ];
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(textureCoordinates),
      gl.STATIC_DRAW,
    );
    return textureCoordBuffer;
  }
function initTetrPositionBuffer(gl) {
    const positionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
    const positions = [
        0.0, 1.0, 0.0,       // Top vertex
        -1.0, -1.0, 1.0,     // Front-left
        1.0, -1.0, 1.0,      // Front-right
        0.0, -1.0, -1.0,     // Back
    ];
    gl.bufferData(
        gl.ARRAY_BUFFER, 
        new Float32Array(positions), 
        gl.STATIC_DRAW
    );
    return positionBuffer;
}
function initTetrIndexBuffer(gl) {
    const indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
    const indices = [
        0, 1, 2,  // Front face
        0, 2, 3,  // Right face
        0, 3, 1,  // Left face
        1, 3, 2,  // Bottom face
    ];
    gl.bufferData(
        gl.ELEMENT_ARRAY_BUFFER, 
        new Uint16Array(indices), 
        gl.STATIC_DRAW
    );
    return indexBuffer;
}
function initTetrTextureBuffer(gl) {
    const textureCoordBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, textureCoordBuffer);
    const textureCoordinates = [
        0.5, 1.0,   // Top vertex
        0.0, 0.0,   // Front-left
        1.0, 0.0,   // Front-right
        0.5, 0.0,   // Back
    ];
    gl.bufferData(
        gl.ARRAY_BUFFER, 
        new Float32Array(textureCoordinates), 
        gl.STATIC_DRAW
    );
    return textureCoordBuffer;
}  