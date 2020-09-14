#version 110
// Simple fragment shader.
// Does texturing and phong shading.

// Parameters from the vertex shader
varying vec3 color;


void main() {
        gl_FragColor.rgb = color; // vec3( 0.3, 0.9, 0.9 );
}
