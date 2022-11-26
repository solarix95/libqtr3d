#version 150
// Simple vertex shader.
// Transforms and projects vertices

// Attributes: Position, normal, texture coordinates
attribute vec4 vertex;
attribute vec3 vnormal;
attribute vec2 vtexcoords;

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4 projection;
uniform mat4 modelview;  // = WorldMatrix * ModelView

// Parameters passed to the fragment shader.
varying vec2 fragTexcoords;
varying vec3 fragNormal;
varying vec4 fragPos;

void main() {
        // "vnormal" is a vector.. we can only transform "points":
        vec4 normPoint = vec4(vertex.x + vnormal.x, vertex.y + vnormal.y, vertex.z + vnormal.z, 1);
        vec4 turnNorm  = modelview * normPoint;
        fragPos        = modelview * vertex;
        fragNormal     = vec3(turnNorm.x - fragPos.x,turnNorm.y - fragPos.y,turnNorm.z - fragPos.z );

        // Standard shader output
        fragTexcoords = vtexcoords;
        gl_Position   = projection * modelview * vertex;
}
