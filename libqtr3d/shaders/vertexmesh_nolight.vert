#version 110
// Simple vertex shader.
// Transforms and projects vertices and calculates parameters for lighting.

// Attributes: Position, normal, texture coordinates
attribute vec4 vertex;
attribute vec3 vnormal;
attribute vec4 vcolor;

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4 projection;
uniform mat4 modelview;

// Parameters passed to the fragment shader.
varying vec4 color;

void main() {
        color = vcolor;

	// Project and send to the fragment shader
        gl_Position = projection * modelview * vertex;
}
