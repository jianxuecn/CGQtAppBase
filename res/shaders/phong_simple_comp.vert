uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

attribute vec3 positionIn; // name "vertex" may cause problem on macOS + Qt 5.9.2
attribute vec3 normalIn; // name "normal" may cause problem on macOS + Qt 5.9.2

varying vec4 fragVertex;
varying vec3 fragNormal;

void main() {
    fragVertex = modelViewMatrix * vec4(positionIn, 1.0);
    gl_Position = projectionMatrix * fragVertex;
    fragNormal = normalMatrix * normalIn;
}
