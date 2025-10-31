#version 430

in vec4 positionBasedColor;
out vec4 color;

void main(void) {
    color = positionBasedColor;
}