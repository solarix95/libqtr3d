#version 110
// Simple fragment shader.
// Does texturing and phong shading.

// Parameters from the vertex shader
varying vec4 color;


void main() {

    // Alpha-Transparency
    if(color.a < 0.1)
        discard;

    gl_FragColor = color;
}
