#version 110
// Simple vertex shader.
// Transforms and projects vertices and calculates parameters for lighting.

// Attributes: Position, normal, texture coordinates
attribute vec4 vertex;
attribute vec3 vnormal;
attribute vec3 vcolor;

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4 projection;
uniform mat4 worldview;
uniform mat4 modelview;
uniform mat4 normalview;
uniform vec3 lightPos;

// Parameters passed to the fragment shader.
varying vec3 color;
varying vec3 eye;
varying vec3 light;
varying vec3 normal;

void main() {
        // vec3 relLightPos = (vec4( -1000.0, 0.0, 0.0, 1.0 ) * modelview).xyz;
        vec3 relLightPos = lightPos; // (vec4( lightPos.x, lightPos.y, lightPos.z, 1.0 ) * worldview).xyz;

        // Texture coordinates are passed through
        color = vcolor;

	// Transform the vertex according to modelview
        vec4 viewVertex = vertex * modelview;

	// Calculate lighting parameters for the fragment shader
        eye    = normalize( viewVertex.xyz );
        light  = normalize( relLightPos - viewVertex.xyz );
        normal = normalize(vec3(vnormal.x,vnormal.y,vnormal.z));// normalize( (vec4(vnormal, 0.0) * normalview ).xyz );
	
	// Project and send to the fragment shader
        gl_Position = projection * viewVertex;
}
