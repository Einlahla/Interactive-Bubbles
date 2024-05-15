#version 120

uniform float u_time;
uniform float u_coodinates;

void main() {
    gl_FragColor = vec4(u_coodinates, 1.0, 0.0, 1.0);
}
