#version 330 core

uniform float time;
uniform vec2 resolution;
out vec3 color;

void main() {
    color = vec3(0, cos(time), sin(time));
}
