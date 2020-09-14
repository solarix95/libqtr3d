#version 110
// Simple vertex shader.
// Transforms and projects vertices and calculates parameters for lighting.

// Attributes: Position, normal, texture coordinates
attribute vec4 vertex;
attribute vec3 vnormal;
attribute vec3 vcolor;

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 normalview;

// Parameters passed to the fragment shader.
varying vec3 color;

void main() {

        color = vcolor;

	// Transform the vertex according to modelview
	vec4 viewVertex;	
	viewVertex = vertex * modelview;
	
	// Project and send to the fragment shader
	gl_Position = viewVertex * projection;
}
