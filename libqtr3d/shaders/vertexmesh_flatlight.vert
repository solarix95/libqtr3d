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
uniform vec3 lightpos;   // = LightPosition * modelview

// Parameters passed to the fragment shader.
varying vec3 color;
varying vec3 normal;
varying vec4 fragpos;
varying vec3 lightPos;

void main() {
        // Lighting
        lightPos = lightpos; // lightpos = modelviewLightpos
        normal   = (modelview * vec4(vnormal.x,vnormal.y,vnormal.z,1)).xyz;
        fragpos  = modelview * vertex;

        // Standard shader output
        color       = vcolor;
        gl_Position = vertex * modelview * projection;
}
