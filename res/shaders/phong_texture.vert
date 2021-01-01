#version 330

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

layout(location = 0) in vec3 positionIn;
layout(location = 1) in vec3 normalIn;
layout(location = 2) in vec2 texCoordIn;

out vec4 fragVertex;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main() {
    fragVertex = modelViewMatrix * vec4(positionIn, 1.0);
    gl_Position = projectionMatrix * fragVertex;
    fragNormal = normalMatrix * normalIn;
    fragTexCoord = texCoordIn;
}
