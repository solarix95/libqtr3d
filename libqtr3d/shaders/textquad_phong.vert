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
uniform mat4 normalview;
uniform mat4 worldview;
uniform vec3 lightPos;

// Parameters passed to the fragment shader.
varying vec2 texcoord;
varying vec3 eye;
varying vec3 light;
varying vec3 normal;

void main() {
        // vec3 lightPos = vec3( 10.0, 10.0, 0.0);
        vec3 relLightPos = (vec4( lightPos.x, lightPos.y, lightPos.z, 1.0 ) * worldview).xyz;
        // vec3 lightPos = (vec4( -1000.0, 1000.0, 0.0, 1.0 ) * modelview).xyz;

	// Texture coordinates are passed through
	texcoord = texcoords;

	// Transform the vertex according to modelview
	vec4 viewVertex;	
        viewVertex = vertex * modelview;

	// Calculate lighting parameters for the fragment shader
	eye = normalize( viewVertex.xyz );
        light = normalize( relLightPos - viewVertex.xyz );
	normal = normalize( (vec4(vnormal, 0.0) * normalview ).xyz );
	
	// Project and send to the fragment shader
	gl_Position = viewVertex * projection;
}
