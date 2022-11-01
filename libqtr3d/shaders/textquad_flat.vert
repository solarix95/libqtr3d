#version 110
// Simple vertex shader.
// Transforms and projects vertices and calculates parameters for lighting.

// Attributes: Position, normal, texture coordinates
attribute vec4 vertex;
attribute vec3 vnormal;
attribute vec2 texcoords;

// Only #version 330 core
// layout(location = 0) in vec4 vertex;
// layout(location = 1) in vec3 vnormal;
// layout(location = 2) in vec2 texcoords;

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 worldview;
uniform mat4 normalview;

// Parameters passed to the fragment shader.
varying vec2 texcoord;

void main() {

	// Texture coordinates are passed through
	texcoord = texcoords;

	// Project and send to the fragment shader
        gl_Position = projection * worldview * modelview * vertex; // vec4(vertex.x, vertex.y, vertex.z, 1.0);
}
