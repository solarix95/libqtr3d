#version 110
// Simple vertex shader.
// Transforms and projects vertices and calculates parameters for lighting.

// Attributes: Position, normal, texture coordinates
attribute vec4 vertex;
attribute vec3 vnormal;
attribute vec2 texcoords;

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4 projection;
uniform mat4 modelview;

// Parameters passed to the fragment shader.
varying vec2 texcoord;

void main() {

	// Texture coordinates are passed through
	texcoord = texcoords;

	// Project and send to the fragment shader
        // gl_Position = projection * worldview * modelview * vertex; // vec4(vertex.x, vertex.y, vertex.z, 1.0);
        // gl_Position = vertex * modelview * projection;
        gl_Position = projection * modelview * vertex;
}
