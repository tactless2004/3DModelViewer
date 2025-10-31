#version 430

layout (location = 0) in vec3 pos;

uniform mat4 mvp;

out vec4 positionBasedColor;

void main(void) {
    gl_Position = mvp * vec4(pos, 1.0);
    positionBasedColor = vec4(pos, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}