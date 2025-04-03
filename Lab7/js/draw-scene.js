function drawScene(gl, programInfo, buffers,rotationX,rotationY,rotationZ,prevX,prevY,prevZ,texture1,texture2,selectedShape) {
    gl.clearColor(0.0, 0.0, 0.0, 1.0); // Clear to black, fully opaque
    gl.clearDepth(1.0); // Clear everything
    gl.enable(gl.DEPTH_TEST); // Enable depth testing
    gl.depthFunc(gl.LEQUAL); // Near things obscure far things
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    const fieldOfView = (45 * Math.PI) / 180; // in radians
    const aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
    const zNear = 0.1;
    const zFar = 100.0;
    const projectionMatrix = mat4.create();
    mat4.perspective(projectionMatrix, fieldOfView, aspect, zNear, zFar);
    let cubeX;let tetrX;
    let cubeY;let tetrY;
    let cubeZ;let tetrZ;
    if(selectedShape == "cube"){
        cubeX = rotationX;tetrX = prevX;
        cubeY = rotationY;tetrY = prevY;
        cubeZ = rotationZ;tetrZ = prevZ;
    }else if(selectedShape == "tetrahedron"){
        tetrX = rotationX;cubeX = prevX;
        tetrY = rotationY;cubeY = prevY;
        tetrZ = rotationZ;cubeZ = prevZ;
    }
    drawFigure(gl,programInfo,buffers.cube,cubeX,cubeY,cubeZ,texture1,projectionMatrix,30,-2);
    drawFigure(gl,programInfo,buffers.tetr,tetrX,tetrY,tetrZ,texture2,projectionMatrix,12,2);
  }
function drawFigure(gl,programInfo,figure,rotationX,rotationY,rotationZ,texture,projectionMatrix,vertNum,move){
    const modelViewMatrix = mat4.create();
    mat4.translate(
        modelViewMatrix, // destination matrix
        modelViewMatrix, // matrix to translate
        [move, 0.0, -6.0],
    ); // amount to translate
    mat4.rotate(
        modelViewMatrix, // destination matrix
        modelViewMatrix, // matrix to rotate
        rotationZ, // amount to rotate in radians
        [0, 0, 1],
      ); // axis to rotate around (Z)
    mat4.rotate(
        modelViewMatrix, // destination matrix
        modelViewMatrix, // matrix to rotate
        rotationY , // amount to rotate in radians
        [0, 1, 0],
      ); // axis to rotate around (Y)
    mat4.rotate(
        modelViewMatrix, // destination matrix
        modelViewMatrix, // matrix to rotate
        rotationX , // amount to rotate in radians
        [1, 0, 0],
      );
    setPositionAttribute(gl, figure, programInfo);
    setTextureAttribute(gl, figure, programInfo);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, figure.indices);
    gl.useProgram(programInfo.program);
    gl.uniformMatrix4fv(
        programInfo.uniformLocations.projectionMatrix,
        false,
        projectionMatrix,
    );
    gl.uniformMatrix4fv(
        programInfo.uniformLocations.modelViewMatrix,
        false,
        modelViewMatrix,
    );
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.uniform1i(programInfo.uniformLocations.uSampler, 0);
    {
        const vertexCount = vertNum;
        const type = gl.UNSIGNED_SHORT;
        const offset = 0;
        gl.drawElements(gl.TRIANGLES, vertexCount, type, offset);
    } 
}
function setPositionAttribute(gl, buffers, programInfo) {
    const numComponents = 3;
    const type = gl.FLOAT;
    const normalize = false;
    const stride = 0; 
    const offset = 0;
    gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
    gl.vertexAttribPointer(
        programInfo.attribLocations.vertexPosition,
        numComponents,
        type,
        normalize,
        stride,
        offset,
    );
    gl.enableVertexAttribArray(programInfo.attribLocations.vertexPosition);
}
function setTextureAttribute(gl, buffers, programInfo) {
    const num = 2; 
    const type = gl.FLOAT; 
    const normalize = false; 
    const stride = 0; 
    const offset = 0; 
    gl.bindBuffer(gl.ARRAY_BUFFER, buffers.textureCoord);
    gl.vertexAttribPointer(
      programInfo.attribLocations.textureCoord,
      num,
      type,
      normalize,
      stride,
      offset,
    );
    gl.enableVertexAttribArray(programInfo.attribLocations.textureCoord);
}
  