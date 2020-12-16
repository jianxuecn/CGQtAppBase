#version 330

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out vec4 fragVertex;
out vec3 fragNormal;

void main() {
    fragVertex = modelViewMatrix * vec4(vertex, 1.0);
    gl_Position = projectionMatrix * fragVertex;
    fragNormal = normalMatrix * normal;
}
