#version 330 core

uniform float time;
uniform vec2 resolution;
out vec3 color;

//This sets the current pixel coordinate
//to always range from float 0 to 1, making
//it easier to work with
vec2 realcoord = gl_FragCoord.xy / resolution;

void main() {

	float vpillars = sin(time + realcoord.x * 16);
	float hpillars = sin(time + realcoord.y * 16);

	color = vec3(vpillars, hpillars, 0);
}
