uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

attribute vec3 positionIn;
attribute vec3 normalIn;
attribute vec2 texCoordIn;

varying vec4 fragVertex;
varying vec3 fragNormal;
varying vec2 fragTexCoord;

void main() {
    fragVertex = modelViewMatrix * vec4(positionIn, 1.0);
    gl_Position = projectionMatrix * fragVertex;
    fragNormal = normalMatrix * normalIn;
    fragTexCoord = texCoordIn;
}
